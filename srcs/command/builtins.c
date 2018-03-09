/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtins.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 01:03:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/27 09:03:41 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	cd_builtin(t_command *cmd)
{
	char		*dir;
	struct stat	infos;

	cmd->cmd_ret = 0;
	dir = cmd->args[1] == NULL ? get_myenv("HOME", cmd->environ) : cmd->args[1];
	if (dir == NULL)
	{
		env_enoent("cd", "HOME");
		return (0);
	}
	if (chdir(dir) == -1)
	{
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
	}
	return (0);
}

static int	print_char(t_quote *quotes, char **str, t_command *cmd)
{
	char	*tmp;

	if (**str == '$' && *((*str) + 1) == '?')
	{
		if ((tmp = ft_itoa(cmd->cmd_ret)) == NULL)
			return (-1);
		ft_putstr(tmp);
		free(tmp);
		*str += 1;
		return (0);
	}
	if ((**str == '<' || **str == '>') && !quotes->doubleq && !quotes->simpleq)
	{
		cmd->cmd_ret = 0;
		ft_putchar('\n');
		return (1);
	}
	if (**str == '\'' && !quotes->doubleq)
		quotes->simpleq = !quotes->simpleq;
	else if (**str == '"' && !quotes->simpleq)
		quotes->doubleq = !quotes->doubleq;
	else
		ft_putchar(**str);
	return (0);
}

static int	echo_builtin(t_command *cmd, char *full_cmd)
{
	int		ret;
	t_quote	quotes;

	quotes.simpleq = 0;
	quotes.doubleq = 0;
	while (*full_cmd != '\0' && *full_cmd != ' ')
		full_cmd += 1;
	full_cmd += 1;
	while (*full_cmd != '\0')
	{
		if ((ret = print_char(&quotes, &full_cmd, cmd)) == -1)
		{
			cmd->cmd_ret = 1;
			return (-1);
		}
		if (ret == 1)
			return (0);
		while (*(full_cmd + 1) != '\0' && *(full_cmd + 1) == ' ')
			full_cmd += 1;
		if (*full_cmd == ' ')
			ft_putchar(' ');
		full_cmd += 1;
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

static void	crop_cmd(char *cmd)
{
	t_quote	quotes;
	char	*pointer;

	pointer = cmd;
	quotes.simpleq = 0;
	quotes.doubleq = 0;
	while (*cmd != '\0')
	{
		if (*cmd == '\'' && !quotes.doubleq)
			quotes.simpleq = !quotes.simpleq;
		else if (*cmd == '"' && !quotes.simpleq)
			quotes.doubleq = !quotes.doubleq;
		if (!quotes.doubleq && !quotes.simpleq && (*cmd == '>' || *cmd == '<'))
		{
			*cmd = '\0';
			cmd -= 1;
			if (ft_strchr(pointer, ' ') != ft_strrchr(pointer, ' '))
				while (*cmd != ' ')
					cmd -= 1;
			while (*cmd == ' ')
				cmd -= 1;
			cmd += 1;
			*cmd = '\0';
			break ;
		}
		cmd += 1;
	}
}

int			exec_builtin(t_command *cmd, char *full_cmd, size_t index, size_t is_last)
{
	int		ret;

	ret = 0;
	crop_cmd(full_cmd);
	if (configure_fd(cmd, index, is_last) == -1)
		return (-1);
	if (close_all_fd(cmd->fd, cmd->fd2) == -1)
		return (-1);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		ret = cd_builtin(cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		ret = echo_builtin(cmd, full_cmd);
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
