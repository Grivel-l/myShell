/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   process.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 23:39:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/31 14:42:06 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

static int	split_and_check(char *content, char ***args,
		t_ret *ret, char ***environ)
{
	t_list	*path_env;

	*args = NULL;
	ret->builtin = 0;
	if ((ft_strsplit_qh(content, ' ', args)) == -1)
		return (-1);
	if (check_builtin(ret, *args, environ) == -1)
		return (-1);
	if (!ret->builtin)
	{
		if ((path_env = get_env(*environ, "PATH", 1)) == NULL)
			return (-1);
		prepare_bin(*args, path_env, *environ, ret);
		ft_lstfree(&path_env);
	}
	if (!ret->stop && ret->ret == -1)
		return (-1);
	return (0);
}

int			exec_commands(t_list *commands, char ***environ, t_ret *ret)
{
	t_list	*begin;
	char	**args;

	begin = commands;
	while (commands != NULL)
	{
		if (split_and_check(commands->content, &args, ret, environ) == -1)
		{
			free_everything(environ, &args, &begin);
			return (-1);
		}
		if (ret->stop)
		{
			free_everything(environ, &args, &begin);
			exit(ret->ret);
		}
		ft_freetab(&args);
		commands = commands->next;
	}
	ft_lstfree(&begin);
	return (0);
}
