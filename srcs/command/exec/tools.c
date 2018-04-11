/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 03:28:11 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/11 17:01:59 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		free_quit(t_list **pointer)
{
	ft_lstfree(pointer);
	return (-1);
}

int		dup_entries(int fd[2], int fd2[2], size_t direction)
{
	if (direction == 0)
	{
		if (dup2(STDIN_FILENO, fd[0]) == -1)
			return (-1);
		if (dup2(STDOUT_FILENO, fd[1]) == -1)
			return (-1);
		if (dup2(STDERR_FILENO, fd2[1]) == -1)
			return (-1);
	}
	else
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			return (-1);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			return (-1);
		if (dup2(fd2[1], STDERR_FILENO) == -1)
			return (-1);
	}
	return (0);
}

int		replace_all(char **line, char **environ, int cmd_ret)
{
	if (replace_cmdret(line, cmd_ret) == -1)
		return (-1);
	if (replace_env(line, environ) == -1)
		return (-1);
	if (replace_tilde(line, environ) == -1)
		return (-1);
	return (0);
}
