/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 03:01:34 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/23 19:11:30 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int			configure_fd(t_command *cmd, size_t is_last, char *content)
{
	if (dup2(cmd->tmp_fd, STDIN_FILENO) == -1)
		return (-1);
	if (!is_last)
		if (dup2(cmd->fd[WRITE_END], STDOUT_FILENO) == -1)
			return (-1);
	return (check_all_heredocs(content));
}
