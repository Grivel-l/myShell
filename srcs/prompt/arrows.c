/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   arrows.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/11 20:39:34 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/23 23:44:31 by legrivel    ###    #+. /#+    ###.fr     */
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
		if (isquoting(prompt->commands))
			return (0);
		if (clear_all(prompt) == -1)
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

static int	end_of_line(t_prompt *prompt)
{
	size_t	i;

	i = prompt->pos - 1;
	while (i > 0 && prompt->commands->content[i] != '\n')
	{
		i -= 1;
		if (put_cap("nd") == -1)
			return (-1);
	}
	if (i == 0)
	{
		while (i <= PL)
		{
			i += 1;
			if (put_cap("nd") == -1)
				return (-1);
		}
	}
	return (0);
}

int			left_arrow(t_prompt *prompt)
{
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
				return (end_of_line(prompt));
			}
		}
		return (put_cap("le"));
	}
	return (0);
}

int			right_arrow(t_prompt *prompt)
{
	int		col;

	if (prompt->line != NULL && prompt->pos < ft_strlen(prompt->line))
	{
		prompt->pos += 1;
		if ((col = tgetnum("co")) == -1)
			return (-1);
		if (prompt->pos + PL != 0 &&
		(((prompt->pos + PL) % col) == 0 ||
		 (prompt->commands != NULL && prompt->commands->content[0] != '\0'
		  && prompt->commands->content[prompt->pos - 1] == '\n')))
			return (put_cap("do"));
		return (put_cap("nd"));
	}
	return (0);
}

int			handle_arrows(t_prompt *prompt)
{
	char	buffer[1];

	buffer[0] = prompt->buffer[2];
	if (buffer[0] == 68)
		return (left_arrow(prompt));
	else if (buffer[0] == 67)
		return (right_arrow(prompt));
	else if (buffer[0] == 70 && prompt->line != NULL)
		return (forward_cursor(prompt, to_line_end_length(prompt)));
	else if (buffer[0] == 72 && prompt->line != NULL)
		return (rewind_cursor(prompt, to_line_start_length(prompt)));
	else if (buffer[0] == 65)
		return (previous_command(prompt));
	else if (buffer[0] == 66)
		return (next_command(prompt));
	return (0);
}
