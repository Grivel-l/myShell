/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/15 19:14:43 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/30 00:56:07 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

pid_t	g_pid;

static void	kill_process(int sig)
{
	kill(g_pid, sig);
}

int			check_bin(char *path, char **bin, char **error)
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

int			split_pipe(char *command, t_command *cmd, t_prompt *prompt)
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
			return (free_quit(&pointer));
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
	if (close(cmd->fd[WRITE_END]) == -1)
		return (-1);
	cmd->tmp_fd = cmd->fd[READ_END];
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
