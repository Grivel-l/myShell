/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtins.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 01:03:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/23 19:12:57 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

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
		ft_checkquotes(&quotes, *cmd);
		if (!quotes.doubleq && !quotes.simpleq &&
			(*cmd == '>' || *cmd == '<' || *cmd == '|'))
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

int			exec_builtin(t_command *cmd, char **full_cmd, size_t is_last)
{
	int		ret;
	int		tmp[2];

	if (pipe(tmp) == -1)
		return (-1);
	ret = 0;
	if (dup2(STDOUT_FILENO, tmp[WRITE_END]) == -1)
		return (-1);
	if (configure_fd(cmd, is_last, *full_cmd) == -1)
		return (-1);
	crop_cmd(*full_cmd);
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
	if (dup2(tmp[WRITE_END], STDOUT_FILENO) == -1)
		return (-1);
	if (close(cmd->fd[WRITE_END]) == -1)
		return (-1);
	cmd->tmp_fd = cmd->fd[READ_END];
	return (ret);
}

int			check_builtins(t_command *cmd)
{
	return (ft_strcmp(cmd->args[0], "cd") == 0 ||
		ft_strcmp(cmd->args[0], "env") == 0 ||
		ft_strcmp(cmd->args[0], "echo") == 0 ||
		ft_strcmp(cmd->args[0], "exit") == 0 ||
		ft_strcmp(cmd->args[0], "setenv") == 0 ||
		ft_strcmp(cmd->args[0], "unsetenv") == 0);
}
