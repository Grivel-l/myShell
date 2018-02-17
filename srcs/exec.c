/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/15 19:14:43 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/17 01:27:46 by legrivel    ###    #+. /#+    ###.fr     */
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

static int	exec_bin(t_command *cmd)
{
	int		ret;
	pid_t	pid;

	if ((pid = fork()) == -1)
		return (0);
	if (pid == 0)
		if (execve(cmd->bin, cmd->args, cmd->environ) == -1)
			return (-1);
	if (pid != 0)
		if (wait(&ret) == -1)
			return (-1);
	return (0);
}

static int	exec_command(char *command, t_command *cmd)
{
	int		ret;

	if (ft_strsplit_qh(command, ' ', &(cmd->args)) == -1)
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
		ret = exec_bin(cmd);
	ft_strdel(&(cmd->bin));
	ft_freetab(&(cmd->args));
	return (ret);
}

static int	split_pipe(char *command, t_command *cmd)
{
	char	**split_tab;
	t_list	*split;
	t_list	*pointer;

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
		if (exec_command(split->content, cmd) == -1)
		{
			ft_lstfree(&pointer);
			return (-1);
		}
		split = split->next;
	}
	ft_lstfree(&pointer);
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
