/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/11 20:46:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/18 00:07:04 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int			write_line(t_prompt *prompt)
{
	size_t	length;

	if (prompt->line != NULL)
	{
		ft_putstr(prompt->line);
		length = ft_strlen(prompt->line) - prompt->pos;
		prompt->pos = ft_strlen(prompt->line);
		return (rewind_cursor(prompt, length));
	}
	return (0);
}

size_t		to_line_start_length(t_prompt *prompt, size_t col)
{
	size_t	pos;

	pos = prompt->pos;
	if (prompt->line[pos] == '\n')
		pos -= 1;
	while (prompt->line[pos] != '\n' && pos > 0)
		pos -= 1;
	if (pos < prompt->pos && prompt->line[pos] == '\n')
		pos += 1;
	if (pos == 0)
		prompt->extra = PL;
	while (prompt->pos - pos > col)
		pos += col;
	return (prompt->pos - pos);
}

size_t		to_line_end_length(t_prompt *prompt)
{
	size_t	pos;

	pos = prompt->pos;
	while (prompt->line[pos] != '\n' && prompt->line[pos] != '\0')
		pos += 1;
	return (pos - prompt->pos);
}

int			clear_all(t_prompt *prompt)
{
	size_t	old_pos;

	old_pos = prompt->pos;
	if (rewind_cursor(prompt, prompt->pos) == -1)
		return (-1);
	prompt->extra = PL;
	while (prompt->extra > 0)
	{
		if (put_cap("le") == -1)
			return (-1);
		prompt->extra -= 1;
	}
	if (put_cap("cd") == -1)
		return (-1);
	if (prompt->quoting || isquoting(prompt->commands))
		ft_putstr("> ");
	else
		ft_putstr("\033[01;32m$\033[0m ");
	prompt->pos = old_pos;
	return (0);
}

int			goto_end(t_prompt *prompt)
{
	size_t	length;

	if (prompt->line != NULL)
	{
		length = ft_strlen(prompt->line);
		while (prompt->line != NULL && prompt->pos < length)
			if (right_arrow(prompt) == -1)
				return (-1);
	}
	return (0);
}

static int	my_putc(int i)
{
	return (write(1, &i, 1));
}

int			put_cap(char *cap)
{
	char	*str;

	if ((str = tgetstr(cap, NULL)) == NULL)
		return (-1);
	if (tputs(str, 1, my_putc) == -1)
		return (-1);
	return (0);
}
