/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   read_stdin.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 02:31:32 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 03:53:26 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static char	*init_values(t_prompt *prompt, char *after, int *ret, char **buffer)
{
	char	*match;

	if ((match = get_after(after)) == NULL)
		return (NULL);
	next_line(&(prompt->line), &(prompt->pos));
	*ret = 0;
	*buffer = NULL;
	prompt->quoting = 1;
	return (match);
}

static int	free_and_quit(char *match)
{
	free(match);
	return (-1);
}

static int	update_buffer(t_prompt *prompt, char **buffer,
		char *match, char *before)
{
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
		return (stop_read(buffer, &match, before));
	free(match);
	next_line(&(prompt->line), &(prompt->pos));
	return (1);
}

static int	check_return(t_prompt *prompt, char *after,
		char **buffer, char *before)
{
	char	*match;

	if (prompt->buffer[0] == 10)
	{
		if ((match = get_after(after)) == NULL)
			return (-1);
		if (prompt->line == NULL)
			if ((prompt->line = ft_strdup("")) == NULL)
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
		return (update_buffer(prompt, buffer, match, before));
	}
	return (0);
}

int			read_set_stdin(char *after, t_prompt *prompt,
		char **environ, char *before)
{
	int		ret;
	char	*match;
	char	*buffer;

	if ((match = init_values(prompt, after, &ret, &buffer)) == NULL)
		return (-1);
	while (ret != 2)
	{
		if (read(STDIN_FILENO, prompt->buffer, 3) == -1)
			return (free_and_quit(match));
		if ((ret = check_return(prompt, after, &buffer, before)) == -1)
			break ;
		if (ret == 0)
		{
			if ((ret = handle_input(prompt, environ)) == 2 || ret == 1)
				if (stop_read(&buffer, &match, before) == -1)
					return (-1);
			if (ret == 2 || ret == 1 || ret == -1)
				break ;
		}
	}
	ft_strdel(&match);
	ft_strdel(&buffer);
	prompt->quoting = 0;
	return (ret == -1 ? -1 : 0);
}
