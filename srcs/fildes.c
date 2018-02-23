/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   fildes.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/22 23:12:54 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/23 01:18:39 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		get_fd(char *str, int default_fd)
{
	char	*pointer;

	pointer = str;
	str += ft_strlen(str);
	while (str != pointer && (*str < '0' || *str > '9'))
		str -= 1;
	return (str == pointer ? default_fd : ft_atoi(str));
}

int		set_stdout_fd(char **args, char *previous)
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
	if (dup2(fd, get_fd(previous, STDOUT_FILENO)) == -1)
	{
		ft_freetab(&args);
		return (-1);
	}
	return (0);
}

int		set_stdin_fd(char **file, char ***args, char *previous)
{
	int		fd;

	if ((fd = open(*file, O_RDONLY)) == -1)
	{
		ft_freetab(&file);
		ft_freetab(args);
		return (-1);
	}
	if (dup2(fd, get_fd(previous, STDIN_FILENO)) == -1)
	{
		close(fd);
		ft_freetab(&file);
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
