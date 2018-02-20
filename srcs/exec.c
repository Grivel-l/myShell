/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/15 19:14:43 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/20 23:40:13 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	path_missing(void)
{
	ft_putstr_fd("$PATH missing", 2);
	return (-1);
}

static int	check_bin(char *path, char **bin, char **error)
{
	int		ret;

	if ((ret = access(path, F_OK)) == -1 && errno != ENOENT)
		return (-1);
	if (ret == -1)
		return (0);
	if ((ret = access(path, X_OK)) == -1 && errno != EACCES)
		return (-1);
	if (ret == -1)
	{
		*error = "Permission denied";
		return (0);
	}
	if ((*bin = ft_strdup(path)) == NULL)
		return (-1);
	*error = NULL;
	return (1);
}

static int	check_paths(char **paths, t_command *cmd)
{
	int		ret;
	char	*tmp;
	char	*error;

	ret = 0;
	tmp = NULL;
	error = NULL;
	while (*paths)
	{
		if ((tmp = ft_strjoin(*paths, "/")) == NULL)
			ret = -1;
		if (ret == 0 && (tmp = ft_strrealloc(tmp, cmd->args[0])) == NULL)
			ret = -1;
		if (ret == 0)
			ret = check_bin(tmp, &(cmd->bin), &error);
		if (ret == -1)
		{
			ft_strdel(&tmp);
			return (-1);
		}
		ft_strdel(&tmp);
		if (ret == 1)
			return (0);
		paths += 1;
	}
	if (error != NULL)
		ft_putstr_fd(error, 2);
	ft_strdel(&error);
	return (0);
}

static int	get_bin_path(t_command *cmd)
{
	char	*path;
	char	**paths;
	char	**pointer;

	if ((path = get_myenv("PATH", cmd->environ)) == NULL)
		return (1);
	if ((paths = ft_strsplit(path, ':')) == NULL)
		return (-1);
	pointer = paths;
	if (check_paths(paths, cmd) == -1)
		return (-1);
	ft_freetab(&pointer);
	return (0);
}

static int	close_all_fd(int fildes[2])
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

static int	exit_all_fd(int fildes[2])
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

static int	exec_bin(t_command *cmd, int fildes[2], size_t is_last)
{
	int		ret;
	pid_t	pid;
	size_t	is_first;

	is_first = fildes[0] == -1 || fildes[1] == -1;
	if (is_first && pipe(fildes) == -1)
		return (-1);
	if ((pid = fork()) == -1)
		return (-1);
	if (pid == 0)
	{
		if (is_first && !is_last && dup2(fildes[1], STDOUT_FILENO) == -1)
			return (-1);
		if (!is_first && is_last && dup2(fildes[0], STDIN_FILENO) == -1)
			return (-1);
		if (close_all_fd(fildes) == -1)
			return (-1);
		if (execve(cmd->bin, cmd->args, cmd->environ) == -1)
			return (-1);
	}
	if (is_last)
	{
		if (close_all_fd(fildes) == -1)
			return (-1);
		if (waitpid(pid, &ret, 0) == -1)
			return (-1);
	}
	return (0);
}

static int	get_side(char *command)
{
	char	*inside;
	char	*outside;

	inside = ft_strrchr(command, '>');
	outside = ft_strrchr(command, '<');
	if (inside != NULL && outside != NULL)
	{
		if (ft_strlen(inside) < ft_strlen(outside))
			outside = NULL;
		else
			inside = NULL;
	}
	if (inside == NULL && outside == NULL)
		return (0);
	else if (inside != NULL)
		return (1);
	else
		return (2);
}

static int	set_fildes(char **args)
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

static int	split_string(t_list *split, char c, char ***args)
{
	int		fd;
	char	**pointer;

	if (ft_strsplit_qh(split->content, c, args) == -1)
		return (-1);
	pointer = *args;
	pointer += 1;
	while (*pointer && (*pointer)[0] != '\0')
	{
		if ((fd = open(*pointer, O_CREAT, 0666)) == -1)
		{
			if (errno == EACCES)
				eacces_error(*pointer, NULL);
			if (errno != EACCES)
			{
				ft_freetab(args);
				return (-1);
			}
		}
		if (close(fd) == -1)
		{
			ft_freetab(args);
			return (-1);
		}
		pointer += 1;
	}
	if (set_fildes(*args) == -1)
		return (-1);
	return (0);
}

static int	crop_args(t_command *cmd, char **args)
{
	char	*str;
	char	*inside;
	char	*outside;
	size_t	length1;
	size_t	length2;

	inside = ft_strchr(*args, '>');
	outside = ft_strchr(*args, '<');
	if (inside == NULL && outside == NULL)
		if ((str = ft_strdup(*args)) == NULL)
			return (-1);
	if (inside != NULL && outside != NULL)
	{
		length1 = ft_strlen(inside);
		length2 = ft_strlen(outside);
		length1 = length1 > length2 ? length2 : length1;
	}
	if ((inside == NULL && outside != NULL) ||
			(outside == NULL && inside != NULL))
		length1 = inside == NULL ? ft_strlen(outside) : ft_strlen(inside);
	if (!(inside == NULL && outside == NULL))
	{
		if ((str = ft_strnew(length1 + 1)) == NULL)
			return (-1);
		ft_strncpy(str, *args, length1);
	}
	ft_freetab(&(cmd->args));
	if (ft_strsplit_qh(str, ' ', &(cmd->args)) == -1)
	{
		free(str);
		return (-1);
	}
	free(str);
	return (0);
}

static int	split_heredoc(t_command *cmd, int fildes[2], t_list *split)
{
	int		ret;
	char	**args;

	if ((ret = get_side(split->content)) == -1)
		return (-1);
	args = NULL;
	if (ret == 0)
		return (exec_bin(cmd, fildes, split->next == NULL));
	else if (ret == 1 && split_string(split, '>', &args) == -1)
		return (-1);
	if (crop_args(cmd, args) == -1)
		return (-1);
	ft_freetab(&args);
	return (exec_bin(cmd, fildes, split->next == NULL));
}

static int	exec_command(t_list *split, t_command *cmd, int fildes[2])
{
	int		ret;

	if (ft_strsplit_qh(split->content, ' ', &(cmd->args)) == -1)
		return (-1);
	if ((ret = get_bin_path(cmd)) == -1)
	{
		ft_freetab(&(cmd->args));
		return (-1);
	}
	if (ret == 1)
	{
		ft_freetab(&(cmd->args));
		return (path_missing());
	}
	if (cmd->bin == NULL)
		not_found(cmd->args[0]);
	else
		ret = split_heredoc(cmd, fildes, split);
	ft_strdel(&(cmd->bin));
	ft_freetab(&(cmd->args));
	if (ret == -1)
		printf("%s for %s\n", strerror(errno), split->content);
	return (ret);
}

static int	split_pipe(char *command, t_command *cmd)
{
	t_list	*split;
	t_list	*pointer;
	int		fildes[2];
	int		old_fd[2];
	char	**split_tab;

	fildes[0] = -1;
	fildes[1] = -1;
	if (pipe(old_fd) == -1)
		return (-1);
	if (dup2(STDIN_FILENO, old_fd[0]) == -1)
		return (-1);
	if (dup2(STDOUT_FILENO, old_fd[1]) == -1)
		return (-1);
	if ((split_tab = ft_strsplit(command, '|')) == NULL)
		return (-1);
	if ((split = ft_tabtolist(split_tab)) == NULL)
	{
		ft_freetab(&split_tab);
		return (-1);
	}
	pointer = split;
	ft_freetab(&split_tab);
	while (split != NULL)
	{
		if (exec_command(split, cmd, fildes) == -1)
		{
			ft_lstfree(&pointer);
			return (exit_all_fd(fildes));
		}
		split = split->next;
	}
	ft_lstfree(&pointer);
	if (dup2(old_fd[0], STDIN_FILENO) == -1)
		return (exit_all_fd(fildes));
	if (dup2(old_fd[1], STDOUT_FILENO) == -1)
		return (exit_all_fd(fildes));
	return (0);
}

int		treate_command(t_prompt *prompt, t_command *cmd)
{
	char	**split_tab;
	t_list	*pointer;
	t_list	*commands;

	if (ft_strsplit_qh(prompt->commands->content, ';', &split_tab) == -1)
		return (-1);
	if ((commands = ft_tabtolist(split_tab)) == NULL)
	{
		ft_freetab(&split_tab);
		return (-1);
	}
	ft_freetab(&split_tab);
	pointer = commands;
	while (commands != NULL)
	{
		if (split_pipe(commands->content, cmd) == -1)
		{
			ft_lstfree(&pointer);
			return (-1);
		}
		commands = commands->next;
	}
	prompt->commands = prompt->commands->next;
	return (0);
}
