/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   treate.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 03:19:43 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/11 16:29:53 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	init_values(t_prompt *prompt, int fd[2],
		int fd2[2], t_list **commands)
{
	char	**split_tab;

	if (pipe(fd) == -1)
		return (-1);
	if (pipe(fd2) == -1)
		return (-1);
	*commands = NULL;
	if (prompt->commands->content[0] != '\0' &&
			ft_strsplit_qh(prompt->commands->content, ';', &split_tab) == -1)
		return (-1);
	if (prompt->commands->content[0] != '\0' && (*split_tab != NULL) &&
			(*commands = ft_tabtolist(split_tab)) == NULL)
	{
		ft_freetab(&split_tab);
		return (-1);
	}
	if (prompt->commands->content[0] != '\0')
		ft_freetab(&split_tab);
	return (0);
}

static int	end_command(t_prompt *prompt, char *old_cmd, int fd[2], int fd2[2])
{
	if (old_cmd == NULL)
		return (-1);
	free(prompt->commands->content);
	prompt->commands->content = old_cmd;
	prompt->commands = prompt->commands->next;
	return (close_all_fd(fd, fd2));
}

static int	prepare_command(struct termios term, char *content,
			int fd[2], int fd2[2])
{
	if (ft_strstr(content, "<<") != NULL)
		if (set_options(term, ICANON | ECHO | ISIG) == -1)
			return (-1);
	return (dup_entries(fd, fd2, 0));
}

static int	reset_command(struct termios term, char *content,
			int fd[2], int fd2[2])
{
	if (dup_entries(fd, fd2, 1) == -1)
		return (-1);
	if (ft_strstr(content, "<<") != NULL)
		if (reset_term(term) == -1)
			return (-1);
	return (0);
}

int			treate_command(t_prompt *prompt, t_command *cmd)
{
	int		fd[2];
	int		fd2[2];
	char	*old_cmd;
	t_list	*pointer;
	t_list	*commands;

	old_cmd = ft_strdup(prompt->commands->content);
	if (replace_all(&(prompt->commands->content),
				cmd->environ, cmd->cmd_ret) == -1)
		return (-1);
	if (init_values(prompt, fd, fd2, &commands) == -1)
		return (-1);
	pointer = commands;
	while (commands != NULL)
	{
		if (prepare_command(prompt->term, commands->content, fd, fd2) == -1)
			return (-1);
		if (split_pipe(commands->content, cmd, prompt) == -1)
			return (free_quit(&pointer));
		if (reset_command(prompt->term, commands->content, fd, fd2) == -1)
			return (-1);
		commands = commands->next;
	}
	ft_lstfree(&pointer);
	return (end_command(prompt, old_cmd, fd, fd2));
}
