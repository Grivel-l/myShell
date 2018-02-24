/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   env_manipulations.c                              .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 01:15:39 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/24 21:18:07 by legrivel    ###    #+. /#+    ###.fr     */
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

int				set_env(t_command *cmd)
{
	int		ret;
	char	*tmp;
	int		index;

	cmd->cmd_ret = 1;
	if (cmd->args[1] == NULL || cmd->args[2] == NULL)
		return (0);
	if ((tmp = ft_strjoin(cmd->args[1], "=")) == NULL)
		return (-1);
	if ((tmp = ft_strrealloc(tmp, cmd->args[2])) == NULL)
		return (-1);
	cmd->cmd_ret = 0;
	if ((index = get_env_index(cmd->environ, cmd->args[1])) == -1)
		ret = ft_pushstr(&(cmd->environ), tmp);
	else
	{
		if (delete_env(&(cmd->environ), index) == -1)
		{
			free(tmp);
			return (-1);
		}
		ret = ft_pushstr(&(cmd->environ), tmp);
	}
	free(tmp);
	return (ret);
}
