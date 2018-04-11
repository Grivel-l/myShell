/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   replace.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/03 17:48:45 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/11 02:06:29 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	re_tilde(char **line, size_t index, char **environ)
{
	char	*new;
	char	*path;

	if ((path = get_myenv("HOME", environ)) == NULL)
	{
		env_enoent("yo", "HOME");
		return (1);
	}
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
		if (*pointer == '~' && *(pointer - 1) == ' ' &&
			(*(pointer + 1) == ' ' || *(pointer + 1) == '\0') &&
			!quotes.simpleq && !quotes.doubleq)
		{
			if ((ret = re_tilde(line, i, environ)) == -1)
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

static int	re_cmdret(char **line, size_t index, int cmd_ret)
{
	char	*new;
	char	*number;

	if ((number = ft_itoa(cmd_ret)) == NULL)
		return (-1);
	if ((new =
ft_strnew(index + ft_strlen(number) + ft_strlen(&((*line)[index + 1])) + 1)) == NULL)
	{
		free(number);
		return (-1);
	}
	ft_strncpy(new, *line, index);
	ft_strcpy(new + index, number);
	ft_strcpy(new + index + ft_strlen(number), (*line) + index + 2);
	free(*line);
	*line = new;
	free(number);
	return (0);
}

static int	replace_cmdret(char **line, int cmd_ret)
{
	size_t	i;
	char	*pointer;

	i = 0;
	pointer = *line;
	while (*pointer)
	{
		if (*pointer == '$' && *(pointer + 1) == '?')
		{
			if (re_cmdret(line, i, cmd_ret) == -1)
				return (-1);
			return (replace_cmdret(line, cmd_ret));
		}
		i += 1;
		pointer += 1;
	}
	return (0);
}

int			replace_all(char **line, char **environ, int cmd_ret)
{
	if (replace_cmdret(line, cmd_ret) == -1)
		return (-1);
	if (replace_env(line, environ) == -1)
		return (-1);
	if (replace_tilde(line, environ) == -1)
		return (-1);
	return (0);
}
