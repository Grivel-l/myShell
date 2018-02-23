/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   heredoc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/21 00:56:10 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/23 01:22:43 by legrivel    ###    #+. /#+    ###.fr     */
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

static char	*get_tmp_file(char *name, size_t i)
{
	if (name == NULL || i > 20)
		return (NULL);
	if (access(name, F_OK) == -1)
		return (name);
	if (access(name, R_OK | W_OK) == -1)
		return (get_tmp_file(ft_strrealloc(name, ft_itoa(i + 1)), i));
	return (name);
}

static int	stop_read(char **buffer, char *match)
{
	int		fd;
	char	*tmp_file;

	if ((tmp_file = get_tmp_file(ft_strdup(TMP_FILE), 0)) == NULL)
		return (-1);
	if ((fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == -1)
	{
		free(tmp_file);
		return (-1);
	}
	write(fd, *buffer, ft_strlen(*buffer) - ft_strlen(match) - 1);
	if (close(fd) == -1)
	{
		free(tmp_file);
		return (-1);
	}
	if ((fd = open(tmp_file, O_RDONLY)) == -1)
	{
		free(tmp_file);
		return (-1);
	}
	free(tmp_file);
	dup2(fd, get_fd(match, STDIN_FILENO));
	return (2);
}

static int	check_return(t_prompt *prompt, char **match, char **buffer)
{
	if (prompt->buffer[0] == 10)
	{
		if (*buffer == NULL)
			*buffer = ft_strdup(prompt->line);
		else
			*buffer = ft_strrealloc(*buffer, prompt->line);
		if (*buffer == NULL)
			return (-1);
		if ((*buffer = ft_strrealloc(*buffer, "\n")) == NULL)
			return (-1);
		ft_putchar('\n');
		if (ft_strcmp(prompt->line, *(match + 1)) == 0)
			return (stop_read(buffer, *(match + 1)));
		next_line(&(prompt->line), &(prompt->pos));
		return (1);
	}
	return (0);
}

static int	read_set_stdin(char **match, t_prompt *prompt, char **buffer)
{
	int		ret;

	next_line(&(prompt->line), &(prompt->pos));
	ret = 0;
	prompt->quoting = 1;
	while (ret != 2)
	{
		if (read(STDIN_FILENO, prompt->buffer, 3) == -1)
			return (-1);
		ret = check_return(prompt, match, buffer);
		if (ret == 0 && handle_input(prompt) == -1)
			ret = -1;
		if (ret == -1)
			break ;
	}
	free(*buffer);
	if (ret == -1)
		return (-1);
	prompt->quoting = 0;
	return (0);
}

static int	update_args(char ***args, char *content, char c)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while (content[i] != '\0' && content[i] != c)
		i += 1;
	if (content[i] == '\0')
		return (0);
	while (content[i] != ' ')
		i -= 1;
	while (content[i] == ' ')
		i -= 1;
	i += 1;
	if ((tmp = ft_strnew(i + 1)) == NULL)
		return (-1);
	ft_strncpy(tmp, content, i);
	ft_freetab(args);
	if (ft_strsplit_qh(tmp, ' ', args) == -1)
		return (-1);
	free(tmp);
	return (0);
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
			{
				eacces_error(*pointer, NULL);
				return (1);
			}
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
		if ((*pointer)[0] != '\0' && set_stdout_fd(*args, **(pointer - 1) == '\0' ? *(pointer - 2) : *(pointer - 1)) == -1)
			return (-1);
		pointer += 1;
	}
	return (update_args(args, split->content, c));
}

static int	set_stdin(t_list *split, char c, char ***args, t_prompt *prompt)
{
	char	**tmp;
	char	*tmp2;
	char	*buffer;
	char	**pointer;

	if (ft_strsplit_qh(split->content, c, args) == -1)
		return (-1);
	pointer = *args;
	pointer += 1;
	buffer = NULL;
	while (*pointer)
	{
		if (ft_strsplit_qh(*pointer, ' ', &tmp) == -1)
			return (-1);
		if (*tmp == NULL)
			ft_freetab(&tmp);
		tmp2 = tmp == NULL ? *pointer : *tmp;
		if (tmp2[0] != '\0' && (*(pointer - 1))[0] != '\0')
		{
			if (access(tmp2, F_OK) == -1)
			{
				ft_freetab(&tmp);
				enoent_error(*pointer, NULL);
				return (1);
			}
			if (set_stdin_fd(tmp == NULL ? pointer : tmp, args, *(pointer - 1)) == -1)
				return (-1);
		}
		if (tmp2[0] == '\0')
			if (read_set_stdin(pointer, prompt, &buffer) == -1)
				return (-1);
		ft_freetab(&tmp);
		pointer += 1;
	}
	return (update_args(args, split->content, c));
}

int			split_heredoc(t_command *cmd, int fildes[2], t_list *split, t_prompt *prompt)
{
	int		ret;
	int		stop;
	char	**args;

	if (ft_strchr(split->content, '>') == NULL &&
			ft_strchr(split->content, '<') == NULL)
		return (exec_bin(cmd, fildes, split->next == NULL));
	if ((ret = get_side(split->content)) == -1)
		return (-1);
	args = NULL;
	if (ret == 0)
		return (exec_bin(cmd, fildes, split->next == NULL));
	else if (ret == 1 && (stop = set_stdout(split, '>', &args)) == -1)
		return (-1);
	else if (ret == 2 && (stop = set_stdin(split, '<', &args, prompt)) == -1)
		return (-1);
	ft_freetab(&(cmd->args));
	cmd->args = args;
	return (stop == 1 ? 0 : exec_bin(cmd, fildes, split->next == NULL));
}
