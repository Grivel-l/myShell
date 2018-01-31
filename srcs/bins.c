/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bins.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/31 01:01:25 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/31 01:04:12 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_bin(char **path, char **args, char **environ, t_ret *ret)
{
	pid_t		pid;

	ret->ret = -1;
	if ((pid = fork()) == -1)
		return (0);
	else if (pid == 0)
		execve(*path, args, environ);
	else
	{
		ret->cmd_ret = 0;
		ret->ret = wait(&(ret->cmd_ret));
		if (ret->ret != -1)
			ret->cmd_ret = WEXITSTATUS(ret->cmd_ret);
	}
	ft_strdel(path);
	if (ret->ret != -1)
		ret->ret = 0;
	else
		return (0);
	return (1);
}

static char	*get_bin_path(char *path, char *bin, t_ret *ret)
{
	char	*tmp;

	if ((tmp = ft_strjoin(path, "/")) == NULL)
	{
		ret->ret = -1;
		return (NULL);
	}
	if ((tmp = ft_strrealloc(tmp, bin)) == NULL)
	{
		ret->ret = -1;
		ft_strdel(&tmp);
		return (NULL);
	}
	return (tmp);
}

static void	print_errors(size_t found, char **eacces, t_ret *ret, char **args)
{
	if (!found && *eacces != NULL)
		eacces_error(*eacces, NULL);
	else if (!found && *eacces == NULL)
		not_found(args[0]);
	ft_strdel(eacces);
	ret->ret = 0;
}

static int	set_error(char **eacces, char **tmp, t_ret *ret)
{
	if (*eacces == NULL && (*eacces = ft_strdup(*tmp)) == NULL)
	{
		free(*tmp);
		ret->ret = -1;
		return (-1);
	}
	return (0);
}

void		prepare_bin(char **args, t_list *path_env,
		char **environ, t_ret *ret)
{
	char		*tmp;
	int			found;
	char		*eacces;
	int			file_error;
	int			perms_error;

	found = 0;
	eacces = NULL;
	while (path_env != NULL && found == 0)
	{
		if ((tmp = get_bin_path(path_env->content, args[0], ret)) == NULL)
			return ;
		if ((file_error = access(tmp, F_OK)) == -1)
			ft_strdel(&tmp);
		if (!file_error && (perms_error = access(tmp, X_OK)) == -1)
			if (set_error(&eacces, &tmp, ret) == -1)
				return ;
		if (!file_error && !perms_error)
			if ((found = exec_bin(&tmp, args, environ, ret)) == 0)
				ft_strdel(&eacces);
		ft_strdel(&tmp);
		path_env = path_env->next;
	}
	print_errors(found, &eacces, ret, args);
}
