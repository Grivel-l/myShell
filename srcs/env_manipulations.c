/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   env_manipulations.c                              .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/31 00:57:01 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/31 17:54:59 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int			get_env_index(char ***environ, char *arg)
{
	int		i;
	int		index;
	char	**pointer;

	i = 0;
	index = -1;
	pointer = *environ;
	while (**environ != NULL)
	{
		if (ft_strncmp(**environ, arg, ft_strlen(arg)) == 0)
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
	if ((index = get_env_index(environ, args[1])) == -1)
		ret->cmd_ret = -1;
	else
	{
		if (delete_env(environ, index) == -1)
			return (-1);
	}
	return (0);
}

int			setenv_builtin(char ***environ, char **args, t_ret *ret)
{
	t_list	*value;

	ret->builtin = 1;
	if (check_args(args, "setenv KEY VALUE", ret) == -1)
		return (0);
	if ((value = ft_lstnew(args[2], ft_strlen(args[2]))) == NULL)
		return (-1);
	if (update_env(args[1], value, environ) == -1)
	{
		ft_lstfree(&value);
		return (-1);
	}
	ft_lstfree(&value);
	ret->cmd_ret = 0;
	return (0);
}
