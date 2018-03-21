/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   movements.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/06 22:54:28 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/20 22:31:49 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	left(t_prompt *prompt)
{
	char	*line;
	size_t	length;

	if (prompt->line == NULL)
		return (0);
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

	if (prompt->line == NULL)
		return (0);
	length = prompt->pos;
	max_length = ft_strlen(prompt->line);
	while (length < max_length &&
		(prompt->line[length] == ' ' || prompt->line[length] == ';'))
		length += 1;
	while (length < max_length &&
			prompt->line[length] != ' ' && prompt->line[length] != ';')
		length += 1;
	return (forward_cursor(prompt, length - prompt->pos));
}

static int	up(t_prompt *prompt)
{
	int		col;
	size_t	pos;
	size_t	tmp;

	if ((col = tgetnum("co")) == -1)
		return (-1);
	pos = prompt->pos;
	while (prompt->line[pos] != '\n' && pos > 0)
		pos -= 1;
	if (prompt->line[pos] == '\n')
	{
		tmp = prompt->pos - pos;
		pos -= 1;
		while (pos > 0 && prompt->line[pos] != '\n')
			pos -= 1;
		if (pos == 0)
			pos -= PL + 1;
		return (rewind_cursor(prompt, prompt->pos - (pos + tmp)));
	}
	else
		return (rewind_cursor(prompt, prompt->pos - pos > (size_t)col ?
					col : prompt->pos - pos));
}

static int	down(t_prompt *prompt)
{
	int		col;
	size_t	pos;
	size_t	tmp;
	size_t	length;

	if (prompt->line == NULL)
		return (0);
	if ((col = tgetnum("co")) == -1)
		return (-1);
	pos = prompt->pos;
	while (prompt->line[pos] != '\n' && pos > 0)
		pos -= 1;
	tmp = pos == 0 ? prompt->pos - pos + PL + 1 : prompt->pos - pos;
	pos = prompt->pos;
	while (prompt->line[pos] != '\n' && prompt->line[pos] != '\0')
		pos += 1;
	if (prompt->line[pos] == '\n')
		return (forward_cursor(prompt, tmp + (pos - prompt->pos)));
	else
	{
		length = ft_strlen(prompt->line);
		return (forward_cursor(prompt, ((size_t)col + prompt->pos) > length ?
					length - prompt->pos : col));
	}
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
