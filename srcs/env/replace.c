/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   replace.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/16 22:59:54 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/17 00:55:05 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static size_t	get_length(char *environ)
{
	size_t	i;

	i = 0;
	while (*environ && *environ != '=')
	{
		i += 1;
		environ += 1;
	}
	return (i);
}

static int		replace(char **line, char *environ, char *pointer, size_t i)
{
	size_t	j;
	char	*new;

	j = 0;
	while (*environ && *environ != '=')
	{
		j += 1;
		environ += 1;
	}
	if (j > i)
		return (replace(line, "", pointer, i));
	environ += 1;
	if ((new = malloc(ft_strlen(environ) + ft_strlen(*line) - i + 1)) == NULL)
		return (-1);
	j = 0;
	while ((*line) + j != pointer)
		j += 1;
	ft_strncpy(new, *line, j - 1);
	ft_strcpy(new + j - 1, environ);
	ft_strcpy(new + j + ft_strlen(environ) - 1, *line + j + i);
	free(*line);
	*line = new;
	return (0);
}

static int		check_env(char **line, char *str, char **environ)
{
	size_t	i;
	size_t	length;

	length = ft_strlen(str);
	while (*environ != NULL)
	{
		i = get_length(*environ);
		while (i > 0)
		{
			if (length == i && ft_strncmp(str, *environ, i) == 0)
				return (replace(line, *environ, str, i));
			else
				i -= 1;
		}
		environ += 1;
	}
	i = 0;
	while (str[i] && str[i] != ' ')
		i += 1;
	return (replace(line, "", str, i));
}

int				replace_env(char **line, char **environ)
{
	char	*line_pointer;

	line_pointer = *line;
	while (*line_pointer)
	{
		if (*line_pointer == '$')
			return (check_env(line, line_pointer + 1, environ));
		line_pointer += 1;
	}
	return (0);
}
