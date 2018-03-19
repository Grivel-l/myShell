/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   arrows.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/11 20:39:34 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/19 17:16:43 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	previous_command(t_prompt *prompt)
{
	if (prompt->commands != NULL)
	{
		if (isquoting(prompt->commands))
			return (0);
		if (clear_all(prompt) == -1)
			return (-1);
		if (prompt->commands->previous != NULL)
			prompt->commands = prompt->commands->previous;
		ft_strdel(&(prompt->line));
		if ((prompt->line = ft_strdup(prompt->commands->content)) == NULL)
			return (-1);
		prompt->pos = ft_strlen(prompt->line);
		ft_putstr(prompt->line);
	}
	return (0);
}

static int	next_command(t_prompt *prompt)
{
	if (prompt->commands != NULL && prompt->commands->content[0] != '\0')
	{
		if (isquoting(prompt->commands))
			return (0);
		if (clear_all(prompt) == -1)
			return (-1);
		if (prompt->commands->next != NULL)
		{
			prompt->commands = prompt->commands->next;
			ft_strdel(&(prompt->line));
			if ((prompt->line = ft_strdup(prompt->commands->content)) == NULL)
				return (-1);
			prompt->pos = ft_strlen(prompt->line);
			ft_putstr(prompt->line);
		}
	}
	return (0);
}

int			left_arrow(t_prompt *prompt)
{
	int		col;
	size_t	to_start;

	if (prompt->pos > 0)
	{
		prompt->pos -= 1;
		if (prompt->commands != NULL && prompt->commands->content != NULL && prompt->commands->content[0] != '\0')
		{
			if (!isquoting(prompt->commands) &&
					prompt->line[prompt->pos] == '\n')
			{
				if (put_cap("up") == -1)
					return (-1);
				if ((col = tgetnum("co")) == -1)
					return (-1);
				to_start = to_line_start_length(prompt, col, 1);
				prompt->pos -= to_start;
				return (forward_cursor(prompt, to_start));
			}
		}
		return (put_cap("le"));
	}
	return (0);
}

int			right_arrow(t_prompt *prompt)
{
	int		col;
	size_t	start;

	if (prompt->line != NULL && prompt->pos < ft_strlen(prompt->line))
	{
		prompt->pos += 1;
		if ((col = tgetnum("co")) == -1)
			return (-1);
		start = to_line_start_length(prompt, col, 0);
		if (prompt->pos == start)
			start += PL;
		if (prompt->line[prompt->pos - 1] == '\n' || start == (size_t)col)
			return (put_cap("do"));
		return (put_cap("nd"));
	}
	return (0);
}

int			handle_arrows(t_prompt *prompt)
{
	char	c;
	int		col;

	c = prompt->buffer[2];
	if (c == 68)
		return (left_arrow(prompt));
	else if (c == 67)
		return (right_arrow(prompt));
	else if (c == 70 && prompt->line != NULL)
		return (forward_cursor(prompt, to_line_end_length(prompt)));
	else if (c == 72 && prompt->line != NULL)
	{
		if ((col = tgetnum("co")) == -1)
			return (-1);
		return (rewind_cursor(prompt, to_line_start_length(prompt, col, 0)));
	}
	else if (c == 65)
		return (previous_command(prompt));
	else if (c == 66)
		return (next_command(prompt));
	return (0);
}
