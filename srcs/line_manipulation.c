/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   line_manipulation.c                              .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/05 12:20:51 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/05 18:09:51 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		insert_char(char **line, char c, size_t *index)
{
	char	*new;

	*index += 1;
	if (*line == NULL)
	{
		if ((new = malloc(2)) == NULL)
			return (-1);
		new[0] = c;
		new[1] = '\0';
		*line = new;
		return (0);
	}
	if ((new = malloc(ft_strlen(*line) + 2)) == NULL)
		return (-1);
	ft_strncpy(new, *line, (*index - 1));
	new[*index - 1] = c;
	ft_strcpy(&(new[*index]), &((*line)[*index - 1]));
	new[ft_strlen(*line) + 1] = '\0';
	free(*line);
	*line = new;
	return (0);
}

int		remove_char(char **line, size_t *index)
{
	char	*new;

	if (ft_strlen(*line) == 1 || *index == 0)
	{
		if (*index == 1)
		{
			*index -= 1;
			ft_strdel(line);
		}
		return (0);
	}
	if ((new = malloc(ft_strlen(*line))) == NULL)
		return (-1);
	ft_strncpy(new, *line, *index - 1);
	ft_strcpy(&(new[*index - 1]), &((*line)[*index]));
	new[ft_strlen(*line) - 1] = '\0';
	free(*line);
	*line = new;
	*index -= 1;
	return (0);
}
