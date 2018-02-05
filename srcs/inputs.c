/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   inputs.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 03:19:24 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/05 21:19:50 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	handle_backspace(char **line, size_t *pos)
{
	if (*pos > 0)
	{
		if (put_cap("le") == -1)
			return (-1);
		if (put_cap("dc") == -1)
			return (-1);
	}
	return (remove_char(line, pos));
}

static int	end_of_line(char **line, size_t *pos, t_dlist **list)
{
	size_t	length;
	char	buffer[3];

	buffer[2] = 67;
	length = ft_strlen(*line);
	while (*pos < length)
	{
		if (handle_arrows(buffer, line, pos, list) == -1)
			return (-1);
	}
	return (0);
}

static int	start_of_line(char **line, size_t *pos, t_dlist **list)
{
	char	buffer[3];

	buffer[2] = 68;
	while (*pos > 0)
		if (handle_arrows(buffer, line, pos, list) == -1)
			return (-1);
	return (0);
}

static int	clear_line(char **line, size_t *pos, t_dlist **list)
{
	if (start_of_line(line, pos, list) == -1)
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
		if (clear_line(line, pos, list) == -1)
			return (-1);
		if ((*line = ft_strdup((*list)->content)) == NULL)
			return (-1);
		*pos = ft_strlen(*line);
		ft_putstr(*line);
		if ((*list)->previous != NULL)
			*list = (*list)->previous;
	}
	return (0);
}

static int	next_command(char **line, size_t *pos, t_dlist **list)
{
	if (*list != NULL)
	{
		if (clear_line(line, pos, list) == -1)
			return (-1);
		if ((*list)->next != NULL)
		{
			if ((*line = ft_strdup((*list)->next->content)) == NULL)
				return (-1);
			*pos = ft_strlen(*line);
			ft_putstr(*line);
			*list = (*list)->next;
		}
	}
	return (0);
}

int			handle_arrows(char buffer[3], char **line, size_t *pos, t_dlist **list)
{
	int		ret;

	ret = 0;
	if (buffer[2] == 68 && *pos > 0)
	{
		*pos -= 1;
		ret = put_cap("le");
	}
	else if (buffer[2] == 67 && *line != NULL && *pos < ft_strlen(*line))
	{
		*pos += 1;
		ret = put_cap("nd");
	}
	else if (buffer[2] == 70 && *line != NULL)
		ret = end_of_line(line, pos, list);
	else if (buffer[2] == 72 && *line != NULL)
		ret = start_of_line(line, pos, list);
	else if (buffer[2] == 65)
		ret = previous_command(line, pos, list);
	else if (buffer[2] == 66)
		ret = next_command(line, pos, list);
	return (ret);
}

static int	handle_return(char **line, t_dlist **list)
{
	t_dlist	*new;

	if ((new = ft_dlstnew(*line)) == NULL)
		return (-1);
	if (*list == NULL)
		*list = new;
	else
	{
		new->previous = *list;
		(*list)->next = new;
		*list = (*list)->next;
	}
	ft_strdel(line);
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
		return (handle_backspace(line, pos));
	else if (buffer[0] == 10)
		return (handle_return(line, list));
	else if (buffer[0] >= 32 && buffer[0] <= 126)
	{
		if (insert_char(line, buffer[0], pos) == -1)
			return (-1);
		ft_putchar(buffer[0]);
	}
	return (0);
}
