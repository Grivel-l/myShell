/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   fildes.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/22 23:12:54 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/27 05:18:48 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

#define READ_END 0
#define WRITE_END 1

static int	set_even(t_command *cmd, size_t is_last)
{
	if (dup2(cmd->fd2[READ_END], STDIN_FILENO) == -1)
		return (-1);
	if (!is_last)
		if (dup2(cmd->fd[WRITE_END], STDOUT_FILENO) == -1)
			return (-1);
	return (0);
}

static int	set_odd(t_command *cmd, size_t is_last)
{
	if (dup2(cmd->fd[READ_END], STDIN_FILENO) == -1)
		return (-1);
	if (!is_last)
		if (dup2(cmd->fd2[WRITE_END], STDOUT_FILENO) == -1)
			return (-1);
	return (0);
}

int		configure_fd(t_command *cmd, size_t index, size_t is_last)
{
	if (index == 0 && !is_last)
		return (dup2(cmd->fd[WRITE_END], STDOUT_FILENO));
	else if (index != 0 && index % 2 == 0)
		return (set_even(cmd, is_last));
	else if (index != 0 && index % 2 != 0)
		return (set_odd(cmd, is_last));
	return (0);
}

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
	if (close(fd) == -1)
		return (-1);
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
	if (close(fd) == -1)
		return (-1);
	return (0);
}

static int	close_fd(int fd[2])
{
	int		ret;
	int		ret2;

	ret = 0;
	ret2 = 0;
	if (fd[0] != -1)
		ret = close(fd[0]);
	if (fd[1] != -1)
		ret2 = close(fd[1]);
	return ((ret == -1 || ret2 == -1) ? -1 : 0);
}

int		close_all_fd(int fd[2], int fd2[2])
{
	int		ret;
	int		ret2;

	ret = close_fd(fd);
	ret2 = close_fd(fd2);
	return ((ret == -1 || ret2 == -1) ? -1 : 0);
}
