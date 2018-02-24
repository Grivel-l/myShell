/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   env_manipulations.c                              .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 01:15:39 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/24 02:43:03 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

char			*get_myenv(char *env, char **environ)
{
	size_t	length;

	length = ft_strlen(env);
	while (*environ)
	{
		if (ft_strncmp(*environ, env, length) == 0 &&
			(*environ)[length] == '=')
			return (&((*environ)[length + 1]));
		environ += 1;
	}
	return (NULL);
}
