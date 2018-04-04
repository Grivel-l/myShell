/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtins.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 01:03:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/03 20:24:54 by legrivel    ###    #+. /#+    ###.fr     */
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
	int		fd[2];

	ret = 0;
	crop_cmd(*full_cmd);
	if (!is_last && configure_builtin_fd(cmd, fd) == -1)
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
	if (!is_last && dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
		return (-1);
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
