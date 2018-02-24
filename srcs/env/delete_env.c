/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   delete_env.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 02:38:11 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/24 03:20:26 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	get_env_index(char **environ, char *arg)
{
	int		i;

	i = 0;
	while (*environ != NULL)
	{
		if (ft_strncmp(*environ, arg, ft_strlen(arg)) == 0)
			return (i);
		i += 1;
		environ += 1;
	}
	return (-1);
}

static int	copy_env(char ***new, char **pointer, char *env)
{
	if ((**new = ft_strdup(env)) == NULL)
	{
		ft_freetab(&pointer);
		return (-1);
	}
	*new += 1;
	return (0);
}

static int	delete_env(char ***environ, int index)
{
	int		i;
	char	**new;
	char	**pointer;
	char	**new_pointer;

	if ((new = malloc(sizeof(char *) * (ft_tablen(*environ) + 1))) == NULL)
		return (-1);
	i = 0;
	new_pointer = new;
	pointer = *environ;
	while (*pointer != NULL)
	{
		if (i++ != index)
			if (copy_env(&new, new_pointer, *pointer) == -1)
				return (-1);
		free(*pointer);
		pointer += 1;
	}
	*new = NULL;
	free(*environ);
	*environ = new_pointer;
	return (0);
}

int			unset_env(t_command *cmd)
{
	int		index;
	char	**args;

	args = cmd->args + 1;
	if (*args == NULL)
		return (0);
	while (*args)
	{
		index = get_env_index(cmd->environ, *args);
		if (index != -1 && delete_env(&(cmd->environ), index) == -1)
			return (-1);
		args += 1;
	}
	return (0);
}
