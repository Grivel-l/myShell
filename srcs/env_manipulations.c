/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   env_manipulations.c                              .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/31 00:57:01 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/31 14:43:13 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

static int	get_env_index(char ***environ, char **args)
{
	int		i;
	int		index;
	char	**pointer;

	i = 0;
	index = -1;
	pointer = *environ;
	while (**environ != NULL)
	{
		if (ft_strncmp(**environ, args[1], ft_strlen(args[1])) == 0)
		{
			index = i;
			break ;
		}
		i += 1;
		*environ += 1;
	}
	*environ = pointer;
	return (index);
}

int			env_builtin(char ***environ, t_ret *ret)
{
	ret->builtin = 1;
	ret->cmd_ret = 0;
	ft_puttab(*environ);
	return (0);
}

int			unsetenv_builtin(char ***environ, char **args, t_ret *ret)
{
	int		index;

	ret->builtin = 1;
	if (args[1] == NULL || (args[1] != NULL && args[2] != NULL))
	{
		ret->cmd_ret = -1;
		print_usage("unsetenv KEY");
		return (0);
	}
	ret->cmd_ret = 0;
	if ((index = get_env_index(environ, args)) == -1)
		ret->cmd_ret = -1;
	else
	{
		if (delete_env(environ, index) == -1)
			return (-1);
	}
	return (0);
}

static int	env_exist(char **environ, char *arg, t_ret *ret)
{
	t_list	*list;

	ret->cmd_ret = -1;
	if ((list = get_env(environ, arg, 0)) == NULL)
	{
		ret->ret = -1;
		return (-1);
	}
	if (((char *)list->content)[0] == '\0')
	{
		ft_lstfree(&list);
		return (0);
	}
	else
	{
		ft_lstfree(&list);
		return (1);
	}
}

int			setenv_builtin(char ***environ, char **args, t_ret *ret)
{
	char	*tmp;
	int		exist;

	ret->builtin = 1;
	if (check_args(args, "setenv KEY VALUE", ret) == -1)
		return (0);
	if ((exist = env_exist(*environ, args[1], ret)) == -1)
		return (-1);
	if (exist == 1)
		return (0);
	ret->cmd_ret = 0;
	if ((tmp = ft_strjoin(args[1], "=")) == NULL)
		return (-1);
	if ((tmp = ft_strrealloc(tmp, args[2])) == NULL)
	{
		free(tmp);
		return (-1);
	}
	if (ft_pushstr(environ, tmp) == -1)
	{
		free(tmp);
		return (-1);
	}
	free(tmp);
	return (0);
}
