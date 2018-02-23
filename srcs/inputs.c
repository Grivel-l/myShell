/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   inputs.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 03:19:24 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/23 02:01:16 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	handle_backspace(t_prompt *prompt)
{
	if (prompt->pos == 0)
		return (0);
	if (clear_all(prompt) == -1)
		return (-1);
	if (remove_char(&(prompt->line), &(prompt->pos)) == -1)
		return (-1);
	return(write_line(prompt));
}

static int	handle_printable(t_prompt *prompt, char c)
{
	if (clear_all(prompt) == -1)
		return (-1);
	if (insert_char(&(prompt->line), c, &(prompt->pos)) == -1)
		return (-1);
	return(write_line(prompt));
}

static int	check_quotes(t_prompt *prompt)
{
	int		ret;
	char	*command;
	t_dlist	*pointer;

	if (prompt->commands == NULL)
	{
		if (!ft_quotesclosed(prompt->line) && handle_printable(prompt, '\n') == -1)
			return (-1);
		return (0);
	}
	pointer = prompt->commands;
	if (prompt->commands->previous != NULL)
		prompt->commands = prompt->commands->previous;
	if (!isquoting(prompt->commands))
	{
		if (!ft_quotesclosed(prompt->line))
			if (handle_printable(prompt, '\n') == -1)
				return (-1);
		prompt->commands = pointer;
		return (0);
	}
	ret = 1;
	if (prompt->line != NULL && (command = ft_strjoin(prompt->commands->content, prompt->line)) == NULL)
			ret = -1;
	if (prompt->line == NULL)
		command = prompt->commands->content;
	if (ret == 1 && !ft_quotesclosed(command))
		if (handle_printable(prompt, '\n') == -1)
			ret = -1;
	if (prompt->line[0] != '\n')
		ft_strdel(&command);
	if (ret == 1 && (prompt->commands->content = ft_strrealloc(prompt->commands->content, prompt->line)) == NULL)
		ret = -1;
	if (ft_quotesclosed(prompt->commands->content))
		ret = 2;
	return (ret);
}

static int	handle_return(t_prompt *prompt)
{
	int		ret;
	t_dlist	*new;

	if (!isquoting(prompt->commands) && prompt->line == NULL)
		return (1);
	while (prompt->commands != NULL && prompt->commands->next != NULL)
		prompt->commands = prompt->commands->next;
	if ((ret = check_quotes(prompt)) == -1)
		return (-1);
	if (ret == 1)
		return (next_line(&(prompt->line), &(prompt->pos)));
	else if (ret == 2)
		return (1);
	if ((new = ft_dlstnew(prompt->line)) == NULL)
		return (-1);
	if (prompt->commands == NULL)
		prompt->commands = new;
	else
	{
		new->previous = prompt->commands->previous;
		new->previous->next = new;
		free(prompt->commands);
		prompt->commands = new;
	}
	if ((prompt->commands->next = ft_dlstnew("")) == NULL)
		return (-1);
	prompt->commands->next->previous = prompt->commands;
	if (isquoting(prompt->commands))
		return (next_line(&(prompt->line), &(prompt->pos)));
	return (1);
}

static int		handle_basics(t_prompt *prompt)
{
	char	buffer[1];

	buffer[0] = prompt->buffer[0];
	if (buffer[0] == 127)
		return (handle_backspace(prompt));
	else if (buffer[0] == 10)
		return (handle_return(prompt));
	else if (buffer[0] >= 32 && buffer[0] <= 126)
		return (handle_printable(prompt, buffer[0]));
	return (0);
}

int			next_line(char **line, size_t *pos)
{
	ft_strdel(line);
	*pos = 0;
	ft_putstr("> ");
	return (0);
}

int		handle_input(t_prompt *prompt)
{
	char	b[1];

	b[0] = prompt->buffer[0];
	if ((b[0] >= 32 && b[0] <= 127) || b[0] == 10)
		return (handle_basics(prompt));
	else if (b[0] == 6 || b[0] == 7 || b[0] == 8)
		return (handle_ccp(prompt));
	else if (b[0] == 18 || b[0] == 20 || b[0] == 23 || b[0] == 5 || b[0] == 27)
		return (handle_movements(prompt));
	else if (b[0] == 3 || b[0] == 4)
		return (handle_signals(prompt));
	return (0);
}
