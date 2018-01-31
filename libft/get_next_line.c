/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/25 19:39:26 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/27 19:32:00 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static int	read_line(t_fd_list *link)
{
	int		ret;
	char	buffer[BUFF_SIZE + 1];

	if ((ret = read(link->fd, buffer, BUFF_SIZE)) == -1)
		return (-1);
	buffer[ret] = '\0';
	if (ret == 0)
		return (0);
	if (link->content == NULL)
	{
		if ((link->content = ft_strdup(buffer)) == NULL)
			return (-1);
	}
	else
	{
		if ((link->content = ft_strrealloc(link->content, buffer)) == NULL)
			return (-1);
	}
	if (ft_strchr(link->content, '\n') == NULL)
		return (read_line(link));
	return (1);
}

static int	add_link(t_fd_list **list, t_fd_list **link, const int fd)
{
	t_fd_list	*list_start;

	if ((*link = malloc(sizeof(t_fd_list))) == NULL)
		return (-1);
	(*link)->fd = fd;
	(*link)->next = NULL;
	(*link)->content = NULL;
	list_start = *list;
	if (list_start == NULL)
		*list = *link;
	else
	{
		while ((*list)->next != NULL)
			(*list) = (*list)->next;
		(*list)->next = *link;
		*list = list_start;
	}
	return (0);
}

static int	get_link(t_fd_list **list_start, t_fd_list **link, const int fd)
{
	t_fd_list	*list;

	list = *list_start;
	while (list != NULL)
	{
		if (list->fd == fd)
		{
			*link = list;
			return (1);
		}
		list = list->next;
	}
	return (0);
}

static int	copy_line(t_fd_list **link, char **line)
{
	size_t	i;
	char	*pointer;

	i = 0;
	pointer = (*link)->content;
	if ((*link)->content == NULL)
		return (0);
	while (pointer[i] != '\n' && pointer[i] != '\0')
		i += 1;
	if ((*line = malloc(sizeof(char) * (i + 1))) == NULL)
		return (-1);
	ft_strncpy(*line, (*link)->content, i);
	(*line)[i] = '\0';
	if ((pointer = ft_strdup(&((*link)->content[i + 1]))) == NULL)
		return (-1);
	ft_strdel(&((*link)->content));
	(*link)->content = pointer;
	return (0);
}

int			get_next_line(const int fd, char **line)
{
	int					ret;
	t_fd_list			*link;
	static t_fd_list	*list = NULL;

	if (fd < 0 || line == NULL)
		return (-1);
	*line = NULL;
	if (list == NULL || get_link(&list, &link, fd) == 0)
		if (add_link(&list, &link, fd) == -1)
			return (-1);
	ret = 1;
	if (link->content == NULL || ft_strchr(link->content, '\n') == NULL)
		if ((ret = read_line(link)) == -1)
			return (-1);
	if (copy_line(&link, line) == -1)
		return (-1);
	if (ret == 0)
	{
		if (link->content != NULL)
			ft_strdel(&(link->content));
		if (*line != NULL && ft_strlen(*line) != 0)
			return (1);
	}
	return (ret);
}
