/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   chdir.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 01:29:39 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/24 01:25:41 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	check_errors(t_command *cmd, char *dir)
{
	struct stat	infos;

	cmd->cmd_ret = 1;
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
	return (0);
}

static int	create_pwd(char ***environ)
{
	char	*pwd;
	char	*tmp;

	if ((tmp = getcwd(NULL, 0)) == NULL)
		return (-1);
	pwd = ft_strjoin("PWD=", tmp);
	free(tmp);
	if (pwd == NULL || add_env(pwd, environ) == -1)
	{
		ft_strdel(&pwd);
		return (-1);
	}
	free(pwd);
	return (0);
}

static int	update_pwd(char *pwd, char ***environ)
{
	char	*env;

	if ((env = ft_strjoin("OLDPWD=", pwd)) == NULL)
		return (-1);
	if (add_env(env, environ) == -1)
	{
		free(env);
		return (-1);
	}
	free(env);
	return (create_pwd(environ));
}

static char	*get_dir(char *arg, char **environ)
{
	if (arg == NULL ||
			(arg != NULL && ft_strcmp(arg, "--") == 0))
		return (get_myenv("HOME", environ));
	else if (ft_strcmp(arg, "-") == 0)
		return (get_myenv("OLDPWD", environ));
	else
		return (arg);
}

int			cd_builtin(t_command *cmd)
{
	char	*dir;
	char	*pwd;

	cmd->cmd_ret = 0;
	dir = get_dir(cmd->args[1], cmd->environ);
	if (dir == NULL)
	{
		env_enoent("cd", (cmd->args[1] != NULL &&
					ft_strcmp(cmd->args[1], "-")) == 0 ? "OLDPWD" : "HOME");
		return (0);
	}
	if (cmd->args[1] != NULL && ft_strcmp(cmd->args[1], "-") == 0)
	{
		ft_putstr(dir);
		ft_putchar('\n');
	}
	if (chdir(dir) == -1)
		return (check_errors(cmd, dir));
	if ((pwd = get_myenv("PWD", cmd->environ)) == NULL)
		return (create_pwd(&(cmd->environ)));
	else
		return (update_pwd(pwd, &(cmd->environ)));
}
