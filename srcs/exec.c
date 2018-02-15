/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/15 19:14:43 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/15 21:36:39 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	path_missing(void)
{
	ft_putstr_fd("$PATH missing", 2);
	return (-1);
}

static int	check_paths(char **paths, char *command)
{
	int		ret;
	char	*tmp;

	ret = 0;
	tmp = NULL;
	while (*paths)
	{
		if ((tmp = ft_strjoin(*paths, "/")) == NULL)
			ret = -1;
		if (ret == 0 && (tmp = ft_strrealloc(tmp, command)) == NULL)
			ret = -1;
		if (ret == -1)
		{
			ft_strdel(&tmp);
			return (-1);
		}
		printf("%s\n", tmp);
		ft_strdel(&tmp);
		paths += 1;
	}
	return (0);
}

static int	get_bin_path(char **bin, char *command, char **environ)
{
	(void)bin;
	char	*path;
	char	**paths;
	char	**pointer;

	if ((path = get_myenv("PATH", environ)) == NULL)
		return (1);
	if ((paths = ft_strsplit(path, ':')) == NULL)
		return (-1);
	pointer = paths;
	if (check_paths(paths, command) == -1)
		return (-1);
	ft_freetab(&pointer);
	return (0);
}

static int	exec_command(char *command, char **environ)
{
	int		ret;
	char	*bin;

	if ((ret = get_bin_path(&bin, command, environ)) == -1)
		return (-1);
	if (ret == 1)
		return (path_missing());
	return (0);
}

static int	split_pipe(char *command, char **environ)
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
		if (exec_command(split->content, environ) == -1)
		{
			ft_lstfree(&pointer);
			return (-1);
		}
		split = split->next;
	}
	pointer = split;
	return (0);
}

int		treate_command(t_prompt *prompt, char **environ)
{
	char	**split_tab;
	t_list	*pointer;
	t_list	*commands;

	if (ft_strsplit_qh(prompt->line, ';', &split_tab) == -1)
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
		if (split_pipe(commands->content, environ) == -1)
		{
			ft_lstfree(&pointer);
			return (-1);
		}
		commands = commands->next;
	}
	prompt->commands = prompt->commands->next;
	return (0);
}
