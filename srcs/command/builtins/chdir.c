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

static char	*get_pwd(void)
{
	char	pwd[PATH_MAX];

	return (getcwd(pwd, PATH_MAX));
}

static int	create_pwd(char ***environ)
{
	char	*pwd;

	if ((pwd = get_pwd()) == NULL)
		return (-1);
	pwd = ft_strjoin("PWD=", pwd); 
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
	return (create_pwd(environ));
}

int			cd_builtin(t_command *cmd)
{
	char	*dir;
	char	*pwd;

	cmd->cmd_ret = 0;
	dir = cmd->args[1] == NULL ? get_myenv("HOME", cmd->environ) : cmd->args[1];
	if (dir == NULL)
	{
		env_enoent("cd", "HOME");
		return (0);
	}
	if (chdir(dir) == -1)
		return (check_errors(cmd, dir));
	if ((pwd = get_myenv("PWD", cmd->environ)) == NULL)
		return (create_pwd(&(cmd->environ)));
	else
		return (update_pwd(pwd, &(cmd->environ)));
}
