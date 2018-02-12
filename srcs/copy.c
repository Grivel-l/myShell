/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   copy.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/06 20:31:23 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 22:00:11 by legrivel    ###    #+. /#+    ###.fr     */
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

int			rewind_cursor(t_prompt *prompt, size_t length)
{
	while (length > 0)
	{
		if (left_arrow(prompt) == -1)
			return (-1);
		length -= 1;
	}
	return (0);
}

int			forward_cursor(t_prompt *prompt, size_t length)
{
	while (length > 0)
	{
		if (right_arrow(prompt) == -1)
			return (-1);
		length -= 1;
	}
	return (0);
}

static int	rewrite_line(t_prompt *prompt)
{
	if (clear_all(prompt) == -1)
		return (-1);
	prompt->pos = ft_strlen(prompt->line);
	return (write_line(prompt));
}

static int	paste(t_prompt *prompt)
{
	int		ret;
	char	*copy;

	ret = 0;
	if (prompt->copy_buffer == NULL)
		return (0);
	if ((copy = ft_strdup(prompt->copy_buffer)) == NULL)
		return (-1);
	if (ret == 0 && cut(&(prompt->line), &(prompt->pos), &(prompt->copy_buffer)) == -1)
		ret = -1;
	if (ret == 0 && (prompt->line = ft_strrealloc(prompt->line, copy)) == NULL)
		ret = -1;
	if (ret == 0 && (prompt->line = ft_strrealloc(prompt->line, prompt->copy_buffer)) == NULL)
		ret = -1;
	if (ret == 0 && rewrite_line(prompt) == -1)
		ret = -1;
	if (ret == 0 && rewind_cursor(prompt, ft_strlen(prompt->copy_buffer)) == -1)
		ret = -1;
	ft_strdel(&(prompt->copy_buffer));
	prompt->copy_buffer = copy;
	return (ret);
}

int			handle_ccp(t_prompt *prompt)
{
	int		ret;

	ret = 0;
	if (prompt->buffer[0] == 6)
		ret = copy(&(prompt->line), &(prompt->pos), &(prompt->copy_buffer));
	else if (prompt->buffer[0] == 7)
		ret = cut(&(prompt->line), &(prompt->pos), &(prompt->copy_buffer));
	else if (prompt->buffer[0] == 8)
		ret = paste(prompt);
	if (ret == 1)
	{
		ret = 0;
		if (rewrite_line(prompt) == -1)
			return (-1);
	}
	return (ret);
}
