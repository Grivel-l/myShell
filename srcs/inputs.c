/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   inputs.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 03:19:24 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 15:55:39 by legrivel    ###    #+. /#+    ###.fr     */
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

int			clear_all(size_t *pos, t_dlist **list)
{
	size_t	old_pos;

	old_pos = *pos;
	*pos += PL;
	if (rewind_cursor(pos, *pos) == -1)
		return (-1);
	if (put_cap("cd") == -1)
		return (-1);
	if (isquoting(*list))
		ft_putstr("> ");
	else
		ft_putstr("$ ");
	*pos = old_pos;
	return (0);
}

static int	handle_backspace(char **line, size_t *pos, t_dlist **list)
{
	if (*pos == 0)
		return (0);
	if (clear_all(pos, list) == -1)
		return (-1);
	if (remove_char(line, pos) == -1)
		return (-1);
	return(write_line(*line, pos));
}

static int	clear_line(char **line, size_t *pos)
{
	if (rewind_cursor(pos, *pos) == -1)
		return (-1);
	if (put_cap("ce") == -1)
		return (-1);
	ft_strdel(line);
	return (0);
}

static int	previous_command(char **line, size_t *pos, t_dlist **list)
{
	if (*list != NULL)
	{
		if (clear_line(line, pos) == -1)
			return (-1);
		if ((*list)->previous != NULL)
			*list = (*list)->previous;
		if ((*line = ft_strdup((*list)->content)) == NULL)
			return (-1);
		*pos = ft_strlen(*line);
		ft_putstr(*line);
	}
	return (0);
}

static int	next_command(char **line, size_t *pos, t_dlist **list)
{
	if (*list != NULL)
	{
		if (clear_line(line, pos) == -1)
			return (-1);
		if ((*list)->next != NULL)
		{
			*list = (*list)->next;
			if ((*line = ft_strdup((*list)->content)) == NULL)
				return (-1);
			*pos = ft_strlen(*line);
			ft_putstr(*line);
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

int			right_arrow(size_t *pos, char *line)
{
	int		col;

	if (line != NULL && *pos < ft_strlen(line))
	{
		if ((col = tgetnum("co")) == -1)
			return (-1);
		*pos += 1;
		if (*pos + PL != 0 && ((*pos + PL) % col) == 0)
			return (put_cap("do"));
		return (put_cap("nd"));
	}
	return (0);
}

int			handle_arrows(char buffer[3], char **line, size_t *pos, t_dlist **list)
{
	if (buffer[2] == 68)
		return (left_arrow(pos));
	else if (buffer[2] == 67)
		return (right_arrow(pos, *line));
	else if (buffer[2] == 70 && *line != NULL)
		return (forward_cursor(pos, ft_strlen(*line) - *pos, *line));
	else if (buffer[2] == 72 && *line != NULL)
		return (rewind_cursor(pos, *pos));
	else if (buffer[2] == 65)
		return (previous_command(line, pos, list));
	else if (buffer[2] == 66)
		return (next_command(line, pos, list));
	return (0);
}

static int	handle_printable(char **line, size_t *pos, char c, t_dlist **list)
{
	if (clear_all(pos, list) == -1)
		return (-1);
	if (insert_char(line, c, pos) == -1)
		return (-1);
	return(write_line(*line, pos));
}

static int	next_line(char **line, size_t *pos)
{
	ft_strdel(line);
	*pos = 0;
	ft_putstr("> ");
	return (0);
}

static int	check_quotes(char **line, t_dlist **list, size_t *pos)
{
	int		ret;
	char	*command;
	t_dlist	*pointer;

	pointer = *list;
	if (*list == NULL)
	{
		if (!ft_quotesclosed(*line))
			if (handle_printable(line, pos, '\n', list) == -1)
				return (-1);
		return (0);
	}
	if ((*list)->previous != NULL)
		*list = (*list)->previous;
	if ((command = get_previous_command(*list)) == NULL)
		return (-1);
	ret = 0;
	if ((command = ft_strrealloc(command, *line)) == NULL)
		ret = -1;
	if (ret == 0 && !ft_quotesclosed(command))
		if (handle_printable(line, pos, '\n', list) == -1)
			ret = -1;
	*list = pointer;
	ft_strdel(&command);
	return (ret);
}

static int	handle_return(char **line, t_dlist **list, size_t *pos)
{
	t_dlist	*new;

	while (*list != NULL && (*list)->next != NULL)
		*list = (*list)->next;
	if (*line == NULL)
		return (1);
	if (check_quotes(line, list, pos) == -1)
		return (-1);
	if ((new = ft_dlstnew(*line)) == NULL)
		return (-1);
	if (*list == NULL)
		*list = new;
	else
	{
		new->previous = (*list)->previous;
		new->previous->next = new;
		free(*list);
		*list = new;
	}
	if (((*list)->next = ft_dlstnew("")) == NULL)
		return (-1);
	(*list)->next->previous = *list;
	if (isquoting(*list))
		return (next_line(line, pos));
	return (1);
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

int		handle_input(char buffer[3], char **line, size_t *pos, t_dlist **list)
{
	if (buffer[0] == 27)
		return (handle_arrows(buffer, line, pos, list));
	else if (buffer[0] == 127)
		return (handle_backspace(line, pos, list));
	else if (buffer[0] == 10)
		return (handle_return(line, list, pos));
	else if (buffer[0] >= 32 && buffer[0] <= 126)
		return (handle_printable(line, pos, buffer[0], list));
	return (0);
}
