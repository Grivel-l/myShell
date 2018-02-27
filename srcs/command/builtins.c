/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtins.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 01:03:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/27 05:09:55 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	cd_builtin(t_command *cmd)
{
	char		*dir;
	struct stat	infos;

	cmd->cmd_ret = 1;
	dir = cmd->args[1] == NULL ? get_myenv("HOME", cmd->environ) : cmd->args[1];
	if (dir == NULL)
	{
		env_enoent("cd", "HOME");
		return (0);
	}
	if ((cmd->cmd_ret = chdir(dir)) == -1)
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
	cmd->cmd_ret = 0;
	return (0);
}

static int	exit_builtin(t_command *cmd)
{
	cmd->exited = 1;
	return (-1);
}

int			exec_builtin(t_command *cmd)
{
	int		ret;

	ret = 0;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		ret =cd_builtin(cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		ret = echo_builtin(cmd);
	else if (ft_strcmp(cmd->args[0], "unsetenv") == 0)
		ret = unset_env(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		cmd->cmd_ret = 0;
		ft_puttab(cmd->environ);
	}
	else if (ft_strcmp(cmd->args[0], "setenv") == 0)
		ret = set_env(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		ret = exit_builtin(cmd);
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
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
