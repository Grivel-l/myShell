/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bin_path.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 03:44:09 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/09 14:19:03 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static char	update_bin(char *bin)
{
	char	c;

	while (*bin != '\0')
	{
		if (*bin == '>' || *bin == '<')
		{
			c = *bin;
			*bin = '\0';
			return (c);
		}
		bin += 1;
	}
	return (0);
}

static int	create_path(char *path, t_command *cmd, char **error)
{
	int		ret;
	char	*tmp;

	ret = 0;
	if ((tmp = ft_strjoin(path, "/")) == NULL)
		ret = -1;
	if (ret == 0 && (tmp = ft_strrealloc(tmp, cmd->args[0])) == NULL)
		ret = -1;
	if (ret == 0)
		ret = check_bin(tmp, &(cmd->bin), error);
	ft_strdel(&tmp);
	return (ret);
}

static int	check_paths(char **paths, t_command *cmd)
{
	char	c;
	int		ret;
	char	*error;

	error = NULL;
	c = update_bin(cmd->args[0]);
	while (*paths)
	{
		if ((ret = create_path(*paths, cmd, &error)) == -1)
			return (-1);
		if (ret == 1)
		{
			if (c != 0)
				cmd->args[0][ft_strlen(cmd->args[0])] = c;
			return (0);
		}
		paths += 1;
	}
	ret = error != NULL ? 2 : 0;
	if (error != NULL)
		denied_error(&error);
	ft_strdel(&error);
	if (c != 0)
		cmd->args[0][ft_strlen(cmd->args[0])] = c;
	return (ret);
}

static int	check_local_bin(t_command *cmd)
{
	char	c;
	int		ret;

	c = update_bin(cmd->args[0]);
	if (ft_strchr(cmd->args[0], '/') == NULL)
		return (0);
	if ((ret = access(cmd->args[0], F_OK)) == -1 && errno != ENOENT)
		return (check_notdir(cmd->args[0]));
	if (ret == -1)
		return (print_bin_error(cmd->args[0], enoent_error));
	if ((ret = access(cmd->args[0], X_OK)) == -1 && errno != EACCES)
		return (-1);
	if (ret == -1)
		return (print_bin_error(cmd->args[0], eacces_error));
	if ((ret = check_filetype(cmd->args[0])) != 0)
		return (ret);
	if ((cmd->bin = ft_strdup(cmd->args[0])) == NULL)
		return (-1);
	if (c != 0)
		cmd->args[0][ft_strlen(cmd->args[0])] = c;
	return (1);
}

int			get_bin_path(t_command *cmd)
{
	int		ret;
	char	*path;
	char	**paths;
	char	**pointer;

	if ((ret = check_builtins(cmd)) == -1)
		return (-1);
	if (ret == 0 && (ret = check_local_bin(cmd)) == -1)
		return (-1);
	if (ret == 1)
		return (2);
	if ((path = get_myenv("PATH", cmd->environ)) == NULL)
		return (1);
	if ((paths = ft_strsplit(path, ':')) == NULL)
		return (-1);
	pointer = paths;
	if ((ret = check_paths(paths, cmd)) == -1)
		return (-1);
	ft_freetab(&pointer);
	return (ret);
}
