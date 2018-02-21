/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   heredoc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/21 00:56:10 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/21 01:04:04 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	get_side(char *command)
{
	char	*inside;
	char	*outside;

	inside = ft_strrchr(command, '>');
	outside = ft_strrchr(command, '<');
	if (inside != NULL && outside != NULL)
	{
		if (ft_strlen(inside) < ft_strlen(outside))
			outside = NULL;
		else
			inside = NULL;
	}
	if (inside == NULL && outside == NULL)
		return (0);
	else if (inside != NULL)
		return (1);
	else
		return (2);
}

static int	set_inside(t_list *split, char c, char ***args)
{
	int		fd;
	char	**pointer;

	if (ft_strsplit_qh(split->content, c, args) == -1)
		return (-1);
	pointer = *args;
	pointer += 1;
	while (*pointer && (*pointer)[0] != '\0')
	{
		if ((fd = open(*pointer, O_CREAT, 0666)) == -1)
		{
			if (errno == EACCES)
				eacces_error(*pointer, NULL);
			if (errno != EACCES)
			{
				ft_freetab(args);
				return (-1);
			}
		}
		if (close(fd) == -1)
		{
			ft_freetab(args);
			return (-1);
		}
		pointer += 1;
	}
	if (set_fildes(*args) == -1)
		return (-1);
	return (0);
}

static int	crop_args(t_command *cmd, char **args, char c)
{
	char	*str;
	char	*inside;
	char	*outside;
	size_t	length1;
	size_t	length2;

	inside = ft_strchr(*args, c);
	outside = ft_strchr(*args, c);
	if (inside == NULL && outside == NULL)
		if ((str = ft_strdup(*args)) == NULL)
			return (-1);
	if (inside != NULL && outside != NULL)
	{
		length1 = ft_strlen(inside);
		length2 = ft_strlen(outside);
		length1 = length1 > length2 ? length2 : length1;
	}
	if ((inside == NULL && outside != NULL) ||
			(outside == NULL && inside != NULL))
		length1 = inside == NULL ? ft_strlen(outside) : ft_strlen(inside);
	if (!(inside == NULL && outside == NULL))
	{
		if ((str = ft_strnew(length1 + 1)) == NULL)
			return (-1);
		ft_strncpy(str, *args, length1);
	}
	ft_freetab(&(cmd->args));
	if (ft_strsplit_qh(str, ' ', &(cmd->args)) == -1)
	{
		free(str);
		return (-1);
	}
	free(str);
	return (0);
}

int			split_heredoc(t_command *cmd, int fildes[2], t_list *split)
{
	int		ret;
	char	**args;

	if ((ret = get_side(split->content)) == -1)
		return (-1);
	args = NULL;
	if (ret == 0)
		return (exec_bin(cmd, fildes, split->next == NULL));
	else if (ret == 1 && set_inside(split, '>', &args) == -1)
		return (-1);
	if (crop_args(cmd, args, ret == 1 ? '>' : '<') == -1)
		return (-1);
	ft_freetab(&args);
	return (exec_bin(cmd, fildes, split->next == NULL));
}
