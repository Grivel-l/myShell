/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 03:01:34 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 03:01:44 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	exit_fd(int fd[2])
{
	close_fd(fd);
	return (-1);
}

int			configure_builtin_fd(t_command *cmd, int tmp[2])
{
	if (pipe(tmp) == -1)
		return (-1);
	if (dup2(STDOUT_FILENO, tmp[WRITE_END]) == -1)
		return (exit_fd(tmp));
	if (dup2(cmd->fd[WRITE_END], STDOUT_FILENO) == -1)
		return (exit_fd(tmp));
	if (close(cmd->fd[WRITE_END]) == -1)
		return (exit_fd(tmp));
	cmd->tmp_fd = cmd->fd[READ_END];
	return (0);
}

int			configure_fd(t_command *cmd, size_t is_last)
{
	if (dup2(cmd->tmp_fd, STDIN_FILENO) == -1)
		return (-1);
	if (!is_last)
		if (dup2(cmd->fd[WRITE_END], STDOUT_FILENO) == -1)
			return (-1);
	if (close(cmd->fd[READ_END]) == -1)
		return (-1);
	return (0);
}
