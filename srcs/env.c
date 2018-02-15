/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   env.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 00:52:21 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/15 21:39:07 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

/* static int		add_link(char *value, t_list **list)
{
	char	*tmp;
	char	*chr;
	size_t	length;

	chr = ft_strchr(value, ':');
	length = ft_strlen(value) - (chr == NULL ? 0 : ft_strlen(chr));
	if ((tmp = malloc(length + 1)) == NULL)
		return (-1);
	ft_strncpy(tmp, value, length);
	tmp[length] = '\0';
	if (*list == NULL)
		*list = ft_lstnew(tmp, ft_strlen(tmp));
	else
		ft_lstappend(list, ft_lstnew(tmp, ft_strlen(tmp)));
	ft_strdel(&tmp);
	if (*list == NULL)
		return (-1);
	return (0);
}

static t_list	*parse_env(char *path_env)
{
	t_list	*values;
	t_list	*begin_list;

	values = NULL;
	begin_list = NULL;
	if (add_link(path_env, &values) == -1)
		return (NULL);
	begin_list = values;
	while (*path_env)
	{
		if (*path_env == ':' && (*path_env + 1) != '\0')
		{
			if (add_link((path_env + 1), &values) == -1)
			{
				ft_lstfree(&begin_list);
				return (NULL);
			}
		}
		path_env += 1;
	}
	return (begin_list == NULL ? values : begin_list);
} */

char			*get_myenv(char *env, char **environ)
{
	size_t	length;

	length = ft_strlen(env);
	while (*environ)
	{
		if (ft_strncmp(*environ, env, length) == 0 && (*environ)[length] == '=')
			return (&((*environ)[length + 1]));
		environ += 1;
	}
	return (NULL);
}

/* static int		copy_env(char ***new, char **pointer, char ***environ)
{
	if ((**new = ft_strdup(**environ)) == NULL)
	{
		ft_freetab(&pointer);
		return (-1);
	}
	*new += 1;
	return (0);
}

int				delete_env(char ***environ, int index)
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
	while (**environ != NULL)
	{
		if (i++ != index)
			if (copy_env(&new, new_pointer, environ) == -1)
				return (-1);
		free(**environ);
		*environ += 1;
	}
	*new = NULL;
	free(pointer);
	*environ = new_pointer;
	return (0);
}*/
