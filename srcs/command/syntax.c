/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   syntax.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/28 01:41:04 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/11 00:27:43 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	quit(t_prompt *prompt, char c)
{
	prompt->commands = prompt->commands->next;
	return (syntax_error(c));
}

static int	set_heredoc(char *line, size_t *heredoc,
		t_quote quotes, t_prompt *prompt)
{
	if (!quotes.simpleq && !quotes.doubleq && (*line == '<' || *line == '>'))
	{
		*heredoc = 1;
		if (*(line + 1) != '<' && *(line + 1) != '>')
			return (0);
		if ((*(line + 1) == '<' || *(line + 1) == '>') && *(line + 2) == '&')
			return (quit(prompt, *(line + 2)));
		if (*(line + 2) == '<' || *(line + 2) == '>')
			return (quit(prompt, *(line + 2)));
	}
	else
		*heredoc = 0;
	return (0);
}

static int	set_pipe(char *line, size_t *pipe, t_quote quotes, t_prompt *prompt)
{
	if (!quotes.simpleq && !quotes.doubleq && *line == '|')
	{
		*pipe = 1;
		if (*(line + 1) != '|')
			return (0);
		else
			return (quit(prompt, '|'));
	}
	else if (*line != ' ')
		*pipe = 0;
	return (0);
}

static int	check_semicolon(char *line, t_quote quotes, t_prompt *prompt)
{
	if (!quotes.simpleq && !quotes.doubleq && *line == ';')
	{
		if (ft_strlen(line) == ft_strlen(prompt->line))
			return (quit(prompt, ';'));
		line += 1;
		while (*line != '\0')
		{
			if (*line != ' ' && *line != ';')
				break ;
			else if (*line == ';')
				return (quit(prompt, ';'));
			line += 1;
		}
		if (line - 1 == prompt->line)
			return (quit(prompt, ';'));
	}
	return (0);
}

int			check_syntax(t_prompt *prompt)
{
	size_t	pipe;
	char	*line;
	t_quote	quotes;
	size_t	heredoc;

	pipe = 0;
	heredoc = 0;
	line = prompt->line;
	while (*line != '\0')
	{
		ft_checkquotes(&quotes, *line);
		if (set_heredoc(line, &heredoc, quotes, prompt) == 1)
			return (1);
		if (set_pipe(line, &pipe, quotes, prompt) == 1)
			return (1);
		if (check_semicolon(line, quotes, prompt) == 1)
			return (1);
		line += 1;
	}
	if (heredoc == 1 || pipe == 1)
		return (quit(prompt, pipe ? '|' : 0));
	return (0);
}
