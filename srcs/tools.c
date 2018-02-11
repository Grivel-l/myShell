/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/11 20:46:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 20:48:43 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int			write_line(char *line, size_t *pos)
{
	size_t	length;

	if (line != NULL)
	{
		ft_putstr(line);
		length = ft_strlen(line) - *pos;
		*pos = ft_strlen(line);
		return (rewind_cursor(pos, length));
	}
	return (0);
}

int			clear_all(size_t *pos, t_dlist *commands)
{
	size_t	old_pos;

	old_pos = *pos;
	*pos += PL;
	if (rewind_cursor(pos, *pos) == -1)
		return (-1);
	if (put_cap("cd") == -1)
		return (-1);
	if (isquoting(commands))
		ft_putstr("> ");
	else
		ft_putstr("$ ");
	*pos = old_pos;
	return (0);
}

int			clear_line(char **line, size_t *pos)
{
	if (rewind_cursor(pos, *pos) == -1)
		return (-1);
	if (put_cap("ce") == -1)
		return (-1);
	ft_strdel(line);
	return (0);
}

static int	my_putc(int i)
{
	ft_putchar(i);
	return (0);
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
