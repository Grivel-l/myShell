/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/15 19:14:43 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/19 18:53:03 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

pid_t	g_pid;

static void	kill_process(int sig)
{
	kill(g_pid, sig);
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
		ft_strdel(error);
		if ((*error = ft_strdup(path)) == NULL)
			return (-1);
		return (0);
	}
	if ((*bin = ft_strdup(path)) == NULL)
		return (-1);
	ft_strdel(error);
	return (1);
}

static char	update_bin(char *bin)
{
	char	c;

	while (*bin != '\0')
	{
		if (*bin == '>' || *bin == '<')
		{
			c = *bin;
			*bin = '\0';
			return (c);
		}
		bin += 1;
	}
	return (0);
}

static int	check_paths(char **paths, t_command *cmd)
{
	char	c;
	int		ret;
	char	*tmp;
	char	*error;

	ret = 0;
	tmp = NULL;
	error = NULL;
	c = update_bin(cmd->args[0]);
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
		{
			if (c != 0)
				cmd->args[0][ft_strlen(cmd->args[0])] = c;
			return (0);
		}
		paths += 1;
	}
	if (error != NULL)
	{
		eacces_error(error, NULL);
		ft_strdel(&error);
		return (2);
	}
	ft_strdel(&error);
	if (c != 0)
		cmd->args[0][ft_strlen(cmd->args[0])] = c;
	return (0);
}

static int	get_bin_path(t_command *cmd)
{
	int		ret;
	char	*path;
	char	**paths;
	char	**pointer;

	if ((ret = check_builtins(cmd)) == -1)
		return (-1);
	if (ret == 1)
		return (2);
	if ((path = get_myenv("PATH", cmd->environ)) == NULL)
		return (1);
	if ((paths = ft_strsplit(path, ':')) == NULL)
		return (-1);
	pointer = paths;
	if ((ret = check_paths(paths, cmd)) == -1)
		return (-1);
	ft_freetab(&pointer);
	return (ret);
}

static int	exec_command(t_list *split, t_command *cmd, t_prompt *prompt)
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
		env_enoent(cmd->args[0], "PATH");
		ft_freetab(&(cmd->args));
		return (0);
	}
	if (ret != 2 && cmd->bin == NULL)
		not_found(cmd->args[0]);
	else
		ret = split_heredoc(cmd, split, prompt);
	ft_strdel(&(cmd->bin));
	return (ret);
}

static int	split_pipe(char *command, t_command *cmd, t_prompt *prompt)
{
	t_list	*split;
	t_list	*pointer;
	char	**split_tab;

	if (ft_strsplit_qh(command, '|', &split_tab) == -1)
		return (-1);
	if ((split = ft_tabtolist(split_tab)) == NULL)
	{
		ft_freetab(&split_tab);
		return (-1);
	}
	pointer = split;
	ft_freetab(&split_tab);
	cmd->tmp_fd = STDIN_FILENO;
	while (split != NULL)
	{
		if (pipe(cmd->fd) == -1)
			return (-1);
		if (exec_command(split, cmd, prompt) == -1)
		{
			ft_lstfree(&pointer);
			return (-1);
		}
		ft_freetab(&(cmd->args));
		split = split->next;
	}
	ft_lstfree(&pointer);
	return (0);
}

int			exec_bin(t_command *cmd, t_list *split)
{
	if (cmd->bin == NULL)
		return (exec_builtin(cmd, split->content, split->next == NULL));
	if ((g_pid = fork()) == -1)
		return (-1);
	if (g_pid == 0)
	{
		if (configure_fd(cmd, split->next == NULL) == -1)
			return (-1);
		if (execve(cmd->bin, cmd->args, cmd->environ) == -1)
			return (-1);
	}
	else
	{
		if (close(cmd->fd[WRITE_END]) == -1)
			return (-1);
		cmd->tmp_fd = cmd->fd[READ_END];
	}
	if (split->next == NULL)
	{
		if (signal(SIGINT, kill_process) == SIG_ERR)
			return (-1);
		if (waitpid(g_pid, &(cmd->cmd_ret), 0) == -1)
			return (-1);
		cmd->cmd_ret = WEXITSTATUS(cmd->cmd_ret);
	}
	return (0);
}

int			treate_command(t_prompt *prompt, t_command *cmd)
{
	int		fd[2];
	int		fd2[2];
	char	**split_tab;
	t_list	*pointer;
	t_list	*commands;

	if (replace_env(&(prompt->commands->content), cmd->environ) == -1)
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	if (pipe(fd2) == -1)
		return (-1);
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
		if (ft_strstr(commands->content, "<<") != NULL)
			if (set_options(prompt->term, ICANON | ECHO | ISIG) == -1)
				return (-1);
		if (dup2(STDIN_FILENO, fd[0]) == -1)
			return (-1);
		if (dup2(STDOUT_FILENO, fd[1]) == -1)
			return (-1);
		if (dup2(STDERR_FILENO, fd2[1]) == -1)
			return (-1);
		if (split_pipe(commands->content, cmd, prompt) == -1)
		{
			ft_lstfree(&pointer);
			return (-1);
		}
		if (dup2(fd[0], STDIN_FILENO) == -1)
			return (-1);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			return (-1);
		if (dup2(fd2[1], STDERR_FILENO) == -1)
			return (-1);
		if (ft_strstr(commands->content, "<<") != NULL)
			if (reset_term(prompt->term) == -1)
				return (-1);
		commands = commands->next;
	}
	ft_lstfree(&pointer);
	prompt->commands = prompt->commands->next;
	return (close_all_fd(fd, fd2));
}

