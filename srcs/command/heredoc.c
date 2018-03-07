/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   heredoc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/21 00:56:10 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/06 03:07:22 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static char	*get_tmp_file(char *name, size_t i)
{
	if (name == NULL || i > 20)
		return (NULL);
	if (access(name, F_OK) == -1)
		return (name);
	if (access(name, R_OK | W_OK) == -1)
	{
		free(name);
		return (get_tmp_file(ft_strrealloc(name, ft_itoa(i + 1)), i));
	}
	return (name);
}

static int	stop_read(char **buffer, char *match, char *before)
{
	int		fd;
	char	*tmp_file;

	if ((tmp_file = get_tmp_file(ft_strdup(TMP_FILE), 0)) == NULL)
	{
		free(match);
		return (-1);
	}
	if ((fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == -1)
	{
		free(match);
		free(tmp_file);
		return (-1);
	}
	write(fd, *buffer, ft_strlen(*buffer) - ft_strlen(match) - 1);
	free(match);
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
	if (dup2(fd, get_output_fd(before, STDIN_FILENO)) == -1)
		return (-1);
	if (close(fd) == -1)
		return (-1);
	return (2);
}

/*static int	is_last_double(char **pointer)
{
	char	**tmp;

	while (*pointer != NULL)
	{
		if (ft_strsplit_qh(*pointer, ' ', &tmp) == -1)
			return (-1);
		if (*tmp == NULL)
			ft_freetab(&tmp);
		if ((tmp == NULL && (*pointer)[0] == '\0') ||
				(tmp != NULL && (*tmp)[0] == '\0'))
		{
			ft_freetab(&tmp);
			return (0);
		}
		ft_freetab(&tmp);
		pointer += 1;
	}
	return (1);
}*/

static int	check_return(t_prompt *prompt, char *after, char **buffer, char *before)
{
	char	*match;

	if (prompt->buffer[0] == 10)
	{
		if ((match = get_after(after)) == NULL)
			return (-1);
		if (ft_strstr(after, "<<") != NULL)
		{
			ft_putchar('\n');
			if (ft_strcmp(prompt->line, match) == 0)
			{
				free(match);
				return (2);
			}
			free(match);
			next_line(&(prompt->line), &(prompt->pos));
			return (1);
		}
		if (*buffer == NULL)
			*buffer = ft_strdup(prompt->line);
		else
			*buffer = ft_strrealloc(*buffer, prompt->line);
		if (*buffer == NULL)
		{
			free(match);
			return (-1);
		}
		if ((*buffer = ft_strrealloc(*buffer, "\n")) == NULL)
		{
			free(match);
			return (-1);
		}
		ft_putchar('\n');
		if (ft_strcmp(prompt->line, match) == 0)
			return (stop_read(buffer, match, before));
		free(match);
		next_line(&(prompt->line), &(prompt->pos));
		return (1);
	}
	return (0);
}

int			read_set_stdin(char *after, t_prompt *prompt, char **environ, char *before)
{
	int		ret;
	char	*match;
	char	*buffer;

	if ((match = get_after(after)) == NULL)
		return (-1);
	next_line(&(prompt->line), &(prompt->pos));
	ret = 0;
	buffer = NULL;
	prompt->quoting = 1;
	while (ret != 2)
	{
		if (read(STDIN_FILENO, prompt->buffer, 3) == -1)
		{
			free(match);
			return (-1);
		}
		ret = check_return(prompt, after, &buffer, before);
		if (ret == 0 && handle_input(prompt, environ) == -1)
			ret = -1;
		if (ret == -1)
			break ;
	}
	free(match);
	free(buffer);
	if (ret == -1)
		return (-1);
	prompt->quoting = 0;
	return (0);
}

static int	update_args(char *str, char ***args)
{
	char	c;
	size_t	i;
	char	*pointer;

	i = 0;
	pointer = str;
	while (*str != '<' && *str != '>' && *str != '\0')
	{
		i += 1;
		str += 1;
	}
	if (*str == '\0')
		return (0);
	while (*str != ' ')
	{
		i -= 1;
		str -= 1;
	}
	while (*str == ' ')
	{
		i -= 1;
		str -= 1;
	}
	c = pointer[i + 1];
	pointer[i + 1] = '\0';
	ft_freetab(args);
	if (ft_strsplit_qh(pointer, ' ', args) == -1)
		return (-1);
	pointer[i + 1] = c;
	return (0);
}

static int	check_type(t_prompt *prompt, char **environ, char *pointer)
{
	if (*pointer == '>' && *(pointer + 1) != '>' && *(pointer - 1) != '>')
		return (smp_out(pointer - 1, pointer + 1));
	else if (*pointer == '>' && *(pointer + 1) == '>')
		return (dbl_out(pointer - 1, pointer + 2));
	else if (*pointer == '<' && *(pointer + 1) != '<' && *(pointer - 1) != '<')
		return (smp_in(pointer - 1, pointer + 1));
	else if (*pointer == '<' && *(pointer + 1) == '<')
		return (dbl_in(prompt, environ, pointer - 1, pointer + 2));
	return (0);
}

int			split_heredoc(t_command *cmd, t_list *split, t_prompt *prompt, size_t index)
{
	size_t	i;
	char	*pointer;

	i = 0;
	pointer = split->content;
	while (*pointer)
	{
		if (check_type(prompt, cmd->environ, pointer) == -1)
			return (-1);
		i += 1;
		pointer += 1;
	}
	if (update_args(split->content, &(cmd->args)) == -1)
		return (-1);
	return (exec_bin(cmd, index, split));
	/*if ((ret = get_side(split->content)) == -1)
		return (-1);
	args = NULL;
	if (ret == 0)
		return (exec_bin(cmd, index, split));
	else if (ret == 1 && (stop = set_stdout(split, '>', &args)) == -1)
		return (-1);
	else if (ret == 2 && (stop = set_stdin(split, '<', &args, prompt, cmd->environ)) == -1)
		return (-1);
	ft_freetab(&(cmd->args));
	cmd->args = args;
	return (stop == 1 ? 0 : exec_bin(cmd, index, split));*/
}
