/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtins.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 01:03:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/24 17:36:15 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	cd_builtin(t_command *cmd)
{
	char		*dir;
	struct stat	infos;

	dir = cmd->args[1] == NULL ? get_myenv("HOME", cmd->environ) : cmd->args[1];
	if (cmd == NULL)
	{
		env_enoent("cd", "HOME");
		return (0);
	}
	if (chdir(dir) == -1)
	{
		if (access(dir, F_OK) == -1)
			enoent_error(dir, "cd: ");
		else
		{
			if (stat(dir, &infos) == -1)
				return (-1);
			if (S_ISDIR(infos.st_mode))
				eacces_error(dir, "cd: ");
			else
				not_dir(dir, "cd: ");
		}
	}
	return (0);
}

static int	echo_builtin(t_command *cmd)
{
	char	**pointer;
	
	pointer = cmd->args;
	pointer += 1;
	while (*pointer)
	{
		ft_putstr(*pointer);
		pointer += 1;
	}
	ft_putchar('\n');
	return (0);
}

int			exec_builtin(t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (cd_builtin(cmd));
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (echo_builtin(cmd));
	else if (ft_strcmp(cmd->args[0], "unsetenv") == 0)
		return (unset_env(cmd));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_puttab(cmd->environ);
	else if (ft_strcmp(cmd->args[0], "setenv") == 0)
		return (set_env(cmd));
	/*else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (exit_builtin(cmd));
	else
		return (env_builtin(cmd));*/
	return (0);
}

int			check_builtins(t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "cd") == 0 ||
		ft_strcmp(cmd->args[0], "env") == 0 ||
		ft_strcmp(cmd->args[0], "echo") == 0 ||
		ft_strcmp(cmd->args[0], "exit") == 0 ||
		ft_strcmp(cmd->args[0], "setenv") == 0 ||
		ft_strcmp(cmd->args[0], "unsetenv") == 0)
		return (1);
	return (0);
}
