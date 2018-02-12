/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   movements.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/06 22:54:28 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/12 12:04:49 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	left(t_prompt *prompt)
{
	char	*line;
	size_t	length;

	line = prompt->line;
	length = prompt->pos > 0 ? prompt->pos - 1 : prompt->pos;
	if (line[length] == ' ' || line[length] == ';')
	{
		while ((line[length] == ' ' || line[length] == ';') && length > 0)
			length -= 1;
		length += 1;
	}
	while (line[length] != ' ' && line[length] != ';' && length > 0)
		length -= 1;
	return (rewind_cursor(prompt, prompt->pos - length));
}

static int	right(t_prompt *prompt)
{
	size_t	length;
	size_t	max_length;

	length = prompt->pos;
	max_length = ft_strlen(prompt->line);
	while ((prompt->line[length] == ' ' || prompt->line[length] == ';') && length < max_length)
		length += 1;
	while (prompt->line[length] != ' ' && prompt->line[length] != ';' && length < max_length)
		length += 1;
	return (forward_cursor(prompt, length - prompt->pos));
}

static int	up(t_prompt *prompt)
{
	int		col;
	size_t	pos;
	size_t	length;

	if ((col = tgetnum("co")) == -1)
		return (-1);
	length = 0;
	pos = prompt->pos;
	while (pos > 0 && prompt->commands->content[pos] != '\n')
	{
		pos -= 1;
		length += 1;
	}
	if (pos > 0)
		pos -= 1;
	while (pos > 0 && prompt->commands->content[pos] != '\n')
		pos -= 1;
	length = prompt->pos - pos + 2;
	return (rewind_cursor(prompt, length));
}

static int	down(t_prompt *prompt)
{
	int		col;
	size_t	length;

	if ((col = tgetnum("co")) == -1)
		return (-1);
	length = ft_strlen(prompt->line);
	return (forward_cursor(prompt, ((size_t)col + prompt->pos) > length ? length - prompt->pos : col));
}

int			handle_movements(t_prompt *prompt)
{
	if (prompt->buffer[0] == 27)
		return (handle_arrows(prompt));
	else if (prompt->buffer[0] == 18)
		return (left(prompt));
	else if (prompt->buffer[0] == 20)
		return (right(prompt));
	else if (prompt->buffer[0] == 23)
		return (up(prompt));
	else if (prompt->buffer[0] == 5)
		return (down(prompt));
	return (0);
}
