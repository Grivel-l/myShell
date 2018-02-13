/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/11 20:46:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/13 14:10:35 by legrivel    ###    #+. /#+    ###.fr     */
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

static size_t	to_start_length(t_prompt *prompt)
{
	char	*tmp;
	char	*str;
	size_t	length;
	size_t	pos_dup;

	tmp = NULL;
	if (prompt->line == NULL && prompt->commands == NULL)
		return (0);
	if (prompt->line != NULL && prompt->commands != NULL && prompt->commands->content != NULL)
		if ((tmp = ft_strjoin(prompt->commands->content, prompt->line)) == NULL)
			return (-1);
	if (prompt->line == NULL)
		str = prompt->commands->content;
	else
		str = tmp == NULL ? prompt->line : tmp;
	length = 0;
	pos_dup = prompt->pos;
	while (pos_dup > 0)
	{
		length += 1;
		pos_dup -= 1;
	}
	free(tmp);
	return (length);
}

int			clear_all(t_prompt *prompt)
{
	size_t	old_pos;

	old_pos = prompt->pos;
	prompt->extra = PL;
	if (rewind_cursor(prompt, to_start_length(prompt)) == -1)
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
