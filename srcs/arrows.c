/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   arrows.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/11 20:39:34 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 20:51:57 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	previous_command(t_prompt *prompt)
{
	if (prompt->commands != NULL)
	{
		if (clear_line(&(prompt->line), &(prompt->pos)) == -1)
			return (-1);
		if (prompt->commands->previous != NULL)
			prompt->commands = prompt->commands->previous;
		if ((prompt->line = ft_strdup(prompt->commands->content)) == NULL)
			return (-1);
		prompt->pos = ft_strlen(prompt->line);
		ft_putstr(prompt->line);
	}
	return (0);
}

static int	next_command(t_prompt *prompt)
{
	if (prompt->commands != NULL)
	{
		if (clear_line(&(prompt->line), &(prompt->pos)) == -1)
			return (-1);
		if (prompt->commands->next != NULL)
		{
			prompt->commands = prompt->commands->next;
			if ((prompt->line = ft_strdup(prompt->commands->content)) == NULL)
				return (-1);
			prompt->pos = ft_strlen(prompt->line);
			ft_putstr(prompt->line);
		}
	}
	return (0);
}

int			left_arrow(size_t *pos)
{
	if (*pos > 0)
	{
		*pos -= 1;
		return (put_cap("le"));
	}
	return (0);
}

int			right_arrow(t_prompt *prompt)
{
	int		col;

	if (prompt->line != NULL && prompt->pos < ft_strlen(prompt->line))
	{
		if ((col = tgetnum("co")) == -1)
			return (-1);
		prompt->pos += 1;
		if (prompt->pos + PL != 0 && ((prompt->pos + PL) % col) == 0)
			return (put_cap("do"));
		return (put_cap("nd"));
	}
	return (0);
}

int			handle_arrows(t_prompt *prompt)
{
	char	buffer[3];

	buffer[2] = prompt->buffer[2];
	if (buffer[2] == 68)
		return (left_arrow(&(prompt->pos)));
	else if (buffer[2] == 67)
		return (right_arrow(prompt));
	else if (buffer[2] == 70 && prompt->line != NULL)
		return (forward_cursor(prompt, ft_strlen(prompt->line) - prompt->pos));
	else if (buffer[2] == 72 && prompt->line != NULL)
		return (rewind_cursor(&(prompt->pos), prompt->pos));
	else if (buffer[2] == 65)
		return (previous_command(prompt));
	else if (buffer[2] == 66)
		return (next_command(prompt));
	return (0);
}
