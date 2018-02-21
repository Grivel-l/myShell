/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   heredoc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/21 00:56:10 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/21 03:38:17 by legrivel    ###    #+. /#+    ###.fr     */
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

static int	set_stdout(t_list *split, char c, char ***args)
{
	int		fd;
	char	**pointer;

	if (ft_strsplit_qh(split->content, c, args) == -1)
		return (-1);
	pointer = *args;
	pointer += 1;
	while (*pointer)
	{
		if ((*pointer)[0] != '\0' && (fd = open(*pointer, O_CREAT, 0666)) == -1)
		{
			if (errno == EACCES)
				eacces_error(*pointer, NULL);
			if (errno != EACCES)
			{
				ft_freetab(args);
				return (-1);
			}
		}
		if ((*pointer)[0] != '\0' && close(fd) == -1)
		{
			ft_freetab(args);
			return (-1);
		}
		pointer += 1;
	}
	if (set_stdout_fd(*args) == -1)
		return (-1);
	return (0);
}

static int	check_return(t_prompt *prompt, char **match, char **buffer)
{
	int		fd;

	if (prompt->buffer[0] == 10)
	{
		if (*buffer == NULL)
			*buffer = ft_strdup(prompt->line);
		else
			*buffer = ft_strrealloc(*buffer, prompt->line);
		if (*buffer == NULL)
			return (-1);
		if (ft_strcmp(prompt->line, *(match + 1)) == 0)
		{
			fd = ft_atoi(*(match - 1));
			fd = fd <= -1 ? STDIN_FILENO : fd;
			write(fd, *buffer, ft_strlen(*buffer));
			return (2);
		}
		next_line(&(prompt->line), &(prompt->pos));
		return (1);
	}
	return (0);
}

static int	read_set_stdin(char **match, t_prompt *prompt, char **buffer)
{
	int		ret;

	next_line(&(prompt->line), &(prompt->pos));
	if (read(STDIN_FILENO, prompt->buffer, 3) == -1)
		return (-1);
	ret = check_return(prompt, match, buffer);
	if (ret == 0 && handle_input(prompt) == -1)
		return (-1);
	if (ret != 2)
		return (read_set_stdin(match, prompt, buffer));
	free(*buffer);
	if (ret == -1)
		return (-1);
	return (0);
}

static int	set_stdin(t_list *split, char c, char ***args, t_prompt *prompt)
{
	char	*buffer;
	char	**pointer;

	if (ft_strsplit_qh(split->content, c, args) == -1)
		return (-1);
	pointer = *args;
	pointer += 1;
	buffer = NULL;
	while (*pointer)
	{
		if ((*pointer)[0] != '\0' && access(*pointer, F_OK) == -1)
		{
			enoent_error(*pointer, NULL);
			return (1);
		}
		if ((*pointer)[0] != '\0')
			if (set_stdin_fd(pointer, args) == -1)
				return (-1);
		if ((*pointer)[0] == '\0')
			if (read_set_stdin(pointer, prompt, &buffer) == -1)
				return (-1);
		pointer += 1;
	}
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

int			split_heredoc(t_command *cmd, int fildes[2], t_list *split, t_prompt *prompt)
{
	int		ret;
	int		stop;
	char	**args;

	if ((ret = get_side(split->content)) == -1)
		return (-1);
	stop = 0;
	args = NULL;
	if (ret == 0)
		return (exec_bin(cmd, fildes, split->next == NULL));
	else if (ret == 1 && set_stdout(split, '>', &args) == -1)
		return (-1);
	else if (ret == 2 && (stop = set_stdin(split, '<', &args, prompt)) == -1)
		return (-1);
	if (stop == 0 && crop_args(cmd, args, ret == 1 ? '>' : '<') == -1)
		return (-1);
	ft_freetab(&args);
	return (stop == 1 ? 0 : exec_bin(cmd, fildes, split->next == NULL));
}
