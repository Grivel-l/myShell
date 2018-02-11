/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   copy.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/06 20:31:23 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/09 19:55:06 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	copy(char **line, size_t *pos, char **copy_buffer)
{
	if (*line == NULL)
		return (0);
	ft_strdel(copy_buffer);
	if ((*copy_buffer = malloc(ft_strlen(&((*line)[*pos])) + 1)) == NULL)
		return (-1);
	ft_strcpy(*copy_buffer, &((*line)[*pos]));
	return (0);
}

static int	cut(char **line, size_t *pos, char **copy_buffer)
{
	char	*copy;
	size_t	length;

	if (*line == NULL)
		return (0);
	ft_strdel(copy_buffer);
	if ((*copy_buffer = malloc(ft_strlen(&((*line)[*pos])) + 1)) == NULL)
		return (-1);
	ft_strcpy(*copy_buffer, &((*line)[*pos]));
	length = ft_strlen(*line) - ft_strlen(&((*line)[*pos]));
	if ((copy = malloc(length + 1)) == NULL)
	{
		free(*copy_buffer);
		return (-1);
	}
	ft_strncpy(copy, *line, length);
	copy[length] = '\0';
	free(*line);
	*line = copy;
	return (1);
}

int			rewind_cursor(size_t *pos, size_t length)
{
	while (length > 0)
	{
		if (left_arrow(pos) == -1)
			return (-1);
		length -= 1;
	}
	return (0);
}

int			forward_cursor(size_t *pos, size_t length, char *line)
{
	while (length > 0)
	{
		if (right_arrow(pos, line) == -1)
			return (-1);
		length -= 1;
	}
	return (0);
}

static int	rewrite_line(char *line, size_t *pos, t_dlist **list)
{
	if (clear_all(pos, list) == -1)
		return (-1);
	*pos = ft_strlen(line);
	return (write_line(line, pos));
}

static int	paste(char **line, size_t *pos, char **copy_buffer, t_dlist **list)
{
	int		ret;
	char	*copy;

	ret = 0;
	if (*copy_buffer == NULL)
		return (0);
	if ((copy = ft_strdup(*copy_buffer)) == NULL)
		return (-1);
	if (ret == 0 && cut(line, pos, copy_buffer) == -1)
		ret = -1;
	if (ret == 0 && (*line = ft_strrealloc(*line, copy)) == NULL)
		ret = -1;
	if (ret == 0 && (*line = ft_strrealloc(*line, *copy_buffer)) == NULL)
		ret = -1;
	if (ret == 0 && rewrite_line(*line, pos, list) == -1)
		ret = -1;
	if (ret == 0 && rewind_cursor(pos, ft_strlen(*copy_buffer)) == -1)
		ret = -1;
	ft_strdel(copy_buffer);
	*copy_buffer = copy;
	return (ret);
}

int			handle_copy_buffer(char buffer[3], char **line,
		size_t *pos, char **copy_buffer, t_dlist **list)
{
	int		ret;

	ret = 0;
	if (buffer[0] == 6)
		ret = copy(line, pos, copy_buffer);
	else if (buffer[0] == 7)
		ret = cut(line, pos, copy_buffer);
	else if (buffer[0] == 8)
		ret = paste(line, pos, copy_buffer, list);
	if (ret == 1)
	{
		ret = 0;
		if (rewrite_line(*line, pos, list) == -1)
			return (-1);
	}
	return (ret);
}
