/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/11 20:46:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/12 13:12:33 by legrivel    ###    #+. /#+    ###.fr     */
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

int			clear_all(t_prompt *prompt)
{
	size_t	old_pos;

	old_pos = prompt->pos;
	prompt->pos += PL;
	if (rewind_cursor(prompt, prompt->pos) == -1)
		return (-1);
	if (put_cap("cd") == -1)
		return (-1);
	if (isquoting(prompt->commands))
		ft_putstr("> ");
	else
		ft_putstr("$ ");
	prompt->pos = old_pos;
	return (0);
}

int			clear_line(t_prompt *prompt)
{
	if (rewind_cursor(prompt, prompt->pos) == -1)
		return (-1);
	if (put_cap("ce") == -1)
		return (-1);
	ft_strdel(&(prompt->line));
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
