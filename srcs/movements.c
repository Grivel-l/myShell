/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   movements.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/06 22:54:28 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/09 16:23:49 by legrivel    ###    #+. /#+    ###.fr     */
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

static int	right(char *line, size_t *pos)
{
	size_t	length;
	size_t	max_length;

	length = *pos;
	max_length = ft_strlen(line);
	while ((line[length] == ' ' || line[length] == ';') && length < max_length)
		length += 1;
	while (line[length] != ' ' && line[length] != ';' && length < max_length)
		length += 1;
	return (forward_cursor(pos, length - *pos, line));
}

static int	up(size_t *pos)
{
	int		col;

	if ((col = tgetnum("co")) == -1)
		return (-1);
	return (rewind_cursor(pos, (size_t)col > *pos ? *pos : col));
}

static int	down(char *line, size_t *pos)
{
	int		col;
	size_t	length;

	if ((col = tgetnum("co")) == -1)
		return (-1);
	length = ft_strlen(line);
	return (forward_cursor(pos, ((size_t)col + *pos) > length ? length - *pos : col, line));
}

int			handle_movements(char key, char *line, size_t *pos)
{
	if (line == NULL)
		return (0);
	if (key == 18)
		return (left(line, pos));
	else if (key == 20)
		return (right(line, pos));
	else if (key == 23)
		return (up(pos));
	else if (key == 5)
		return (down(line, pos));
	return (0);
}
