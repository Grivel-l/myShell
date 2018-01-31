/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtins.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 16:14:57 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/31 00:57:52 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_builtin(char **args, t_ret *ret)
{
	ret->stop = 1;
	ret->builtin = 1;
	ret->ret = args[1] == NULL ? ret->cmd_ret : ft_atoi(args[1]);
}

static int	cd_builtin(char **args, char ***environ, t_ret *ret)
{
	char		*dir;
	struct stat	infos;
	t_list		*default_dir;

	ret->cmd_ret = 0;
	ret->builtin = 1;
	if ((default_dir = get_env(*environ, "HOME", 0)) == NULL)
		return (-1);
	dir = args[1] == NULL ? default_dir->content : args[1];
	if (chdir(dir) == -1)
	{
		ret->cmd_ret = 1;
		if (access(dir, F_OK) == -1)
			enoent_error(dir, "cd: ");
		else
		{
			if (stat(dir, &infos) == -1)
				return (-1);
			!S_ISDIR(infos.st_mode) ?
				not_dir(dir, "cd: ") : eacces_error(dir, "cd: ");
		}
	}
	ft_lstfree(&default_dir);
	return (0);
}

static int	echo_builtin(char **args, t_ret *ret)
{
	size_t	i;
	char	**pointer;

	i = 0;
	pointer = args;
	ret->builtin = 1;
	ret->cmd_ret = 0;
	while (*args != NULL)
	{
		if (i > 0)
			ft_putchar(' ');
		ft_putstr(*args);
		i += 1;
		args += 1;
	}
	args = pointer;
	ft_putchar('\n');
	return (0);
}

int			check_builtin(t_ret *ret, char **args, char ***environ)
{
	if (ft_strcmp(args[0], "exit") == 0)
		exit_builtin(args, ret);
	else if (ft_strcmp(args[0], "cd") == 0)
		return (cd_builtin(args, environ, ret));
	else if (ft_strcmp(args[0], "env") == 0)
		return (env_builtin(environ, ret));
	else if (ft_strcmp(args[0], "setenv") == 0)
		return (setenv_builtin(environ, args, ret));
	else if (ft_strcmp(args[0], "unsetenv") == 0)
		return (unsetenv_builtin(environ, args, ret));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (echo_builtin(&(args[1]), ret));
	return (0);
}
