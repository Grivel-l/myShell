/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   heredoc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/21 00:56:10 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/09 19:33:32 by legrivel    ###    #+. /#+    ###.fr     */
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

static int	quit(char *str1, char *str2, int fd)
{
	if (str1 != NULL)
		free(str1);
	if (str2 != NULL)
		free(str2);
	if (fd != -1)
		close(fd);
	return (-1);
}

int			stop_read(char **buffer, char **match, char *before)
{
	int		fd;
	char	*tmp_file;

	if ((tmp_file = get_tmp_file(ft_strdup(TMP_FILE), 0)) == NULL)
		return (quit(*match, NULL, -1));
	if ((fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == -1)
		return (quit(*match, tmp_file, -1));
	if (*buffer != NULL)
		write(fd, *buffer, ft_strlen(*buffer) - ft_strlen(*match) - 1);
	ft_strdel(match);
	if (close(fd) == -1)
		return (quit(NULL, tmp_file, -1));
	if ((fd = open(tmp_file, O_RDONLY)) == -1)
		return (quit(NULL, tmp_file, -1));
	free(tmp_file);
	if (dup2(fd, get_output_fd(before, STDIN_FILENO)) == -1)
		return (quit(NULL, NULL, fd));
	if (close(fd) == -1)
		return (-1);
	return (2);
}

int			set_str(char **str, size_t *i)
{
	while (**str != '<' && **str != '>' && **str != '\0')
	{
		*i += 1;
		*str += 1;
	}
	if (**str == '\0')
		return (1);
	while (**str != ' ' && *i >= 1)
	{
		*i -= 1;
		*str -= 1;
	}
	while (**str == ' ' && *i >= 1)
	{
		*i -= 1;
		*str -= 1;
	}
	return (0);
}
