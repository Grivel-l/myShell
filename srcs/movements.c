/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   movements.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/06 22:54:28 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 20:26:59 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	left(char *line, size_t *pos)
{
	size_t	length;

	length = *pos > 0 ? *pos - 1 : *pos;
	if (line[length] == ' ' || line[length] == ';')
	{
		while ((line[length] == ' ' || line[length] == ';') && length > 0)
			length -= 1;
		length += 1;
	}
	while (line[length] != ' ' && line[length] != ';' && length > 0)
		length -= 1;
	return (rewind_cursor(pos, *pos - length));
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

static int	up(size_t *pos)
{
	int		col;

	if ((col = tgetnum("co")) == -1)
		return (-1);
	return (rewind_cursor(pos, (size_t)col > *pos ? *pos : col));
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
	if (prompt->line == NULL)
		return (0);
	if (prompt->buffer[0] == 18)
		return (left(prompt->line, &(prompt->pos)));
	else if (prompt->buffer[0] == 20)
		return (right(prompt));
	else if (prompt->buffer[0] == 23)
		return (up(&(prompt->pos)));
	else if (prompt->buffer[0] == 5)
		return (down(prompt));
	else if (prompt->buffer[0] == 27)
		return (handle_arrows(prompt));
	return (0);
}
