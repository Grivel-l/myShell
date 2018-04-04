/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   replace.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/03 17:48:45 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/04 05:39:52 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static char	*get_home_path(char *command, char **environ)
{
	char	*path;

	if ((path = get_myenv("HOME", environ)) == NULL)
	{
		env_enoent(command, "HOME");
		return (NULL);
	}
	return (path);
}

static int	join_args(char **str, char **args)
{
	char	**pointer;

	ft_strdel(str);
	if ((*str = ft_strnew(1)) == NULL)
		return (-1);
	pointer = args;
	while (*args != NULL)
	{
		if (((*str)[0] != '\0' && ft_addchar(str, ' ') == -1) ||
				(*str = ft_strrealloc(*str, *args)) == NULL)
		{
			ft_freetab(&pointer);
			return (-1);
		}
		args += 1;
	}
	ft_freetab(&pointer);
	return (0);
}

static int	quit(char *str)
{
	free(str);
	return (-1);
}

int			replace_builtin_tilde(char **str, char **environ)
{
	char	**args;

	if (ft_strsplit_qh(*str, ' ', &args) == -1)
		return (-1);
	if (replace_tilde(args, environ) == -1)
	{
		ft_freetab(&args);
		return (-1);
	}
	return (join_args(str, args));
}

int			replace_tilde(char **args, char **environ)
{
	char	*tmp;
	char	*path;
	char	**pointer;

	if ((path = get_home_path(args[0], environ)) == NULL)
		return (0);
	pointer = args;
	while (*pointer)
	{
		if ((*pointer)[0] == '~' && ((*pointer)[1] == '\0' || (*pointer)[1] == '/'))
		{
			tmp = *pointer;
			if ((*pointer = ft_strdup(path)) == NULL)
				return (quit(path));
			if ((*pointer = ft_strrealloc(*pointer, &(tmp[1]))) == NULL)
				return (quit(path));
			ft_strdel(&tmp);
		}
		pointer += 1;
	}
	return (0);
}
