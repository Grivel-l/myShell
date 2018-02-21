/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   fildes.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/21 01:03:35 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/21 04:18:18 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		set_stdout_fd(char **args)
{
	int		fd;
	int		flags;
	size_t	length;

	length = ft_tablen(args);
	if (length == 1)
		return (0);
	flags = args[length - 2][0] == '\0' ? O_RDWR | O_APPEND : O_RDWR | O_TRUNC;
	if ((fd = open(args[length - 1], flags)) == -1)
	{
		if (errno == EACCES)
			return (0);
		ft_freetab(&args);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_freetab(&args);
		return (-1);
	}
	return (0);
}

int		set_stdin_fd(char **file, char ***args)
{
	int		fd;
	int		target;

	if ((fd = open(*file, O_RDONLY)) == -1)
	{
		ft_freetab(args);
		return (-1);
	}
	target = ft_atoi((*file - 1));
	target = target <= 0 ? STDIN_FILENO : target;
	if (dup2(fd, target) == -1)
	{
		close(fd);
		ft_freetab(args);
		return (-1);
	}
	return (0);
}

int		close_all_fd(int fildes[2])
{
	int		ret1;
	int		ret2;

	ret1 = 0;
	ret2 = 0;
	if (fildes[0] != -1)
	{
		ret1 = close(fildes[0]);
		fildes[0] = -1;
	}
	if (fildes[1] != -1)
	{
		ret2 = close(fildes[1]);
		fildes[1] = -1;
	}
	return ((ret1 == -1 || ret2 == -1) ? -1 : 0);
}

int		exit_all_fd(int fildes[2])
{
	if (fildes[0] != -1)
	{
		close(fildes[0]);
		fildes[0] = -1;
	}
	if (fildes[1] != -1)
	{
		close(fildes[1]);
		fildes[1] = -1;
	}
	return (-1);
}
