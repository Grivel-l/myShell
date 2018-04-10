/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   replace.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/03 17:48:45 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/11 01:35:01 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static char	*get_home_path(char *command, char **environ)
{
	char	*path;

	if ((path = get_myenv("HOME", environ)) == NULL)
		env_enoent(command, "HOME");
	return (path);
}

static int	replace_char(char **line, size_t index, char **environ)
{
	char	*new;
	char	*path;

	if ((path = get_home_path("yo", environ)) == NULL)
		return (1);
	if ((new =
ft_strnew(index + ft_strlen(path) + ft_strlen(&((*line)[index + 1])) + 1)) == NULL)
		return (-1);
	ft_strncpy(new, *line, index);
	ft_strcpy(new + index, path);
	ft_strcpy(new + index + ft_strlen(path), (*line) + index + 1);
	free(*line);
	*line = new;
	return (0);
}

static int	replace_tilde(char **line, char **environ)
{
	size_t	i;
	int		ret;
	t_quote	quotes;
	char	*pointer;

	i = 0;
	pointer = *line;
	quotes.simpleq = 0;
	quotes.doubleq = 0;
	while (*pointer)
	{
		ft_checkquotes(&quotes, *pointer);
		if (!quotes.simpleq && !quotes.doubleq && *pointer == '~')
		{
			if ((ret = replace_char(line, i, environ)) == -1)
				return (-1);
			if (ret == 1)
				return (1);
			return (replace_tilde(line, environ));
		}
		i += 1;
		pointer += 1;
	}
	return (0);
}

int			replace_all(char **line, char **environ, int cmd_ret)
{
	(void)cmd_ret;
	if (replace_env(line, environ) == -1)
		return (-1);
	if (replace_tilde(line, environ) == -1)
		return (-1);
	return (0);
}
