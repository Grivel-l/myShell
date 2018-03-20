/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   completion.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/05 12:10:20 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/20 21:26:15 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int		complete_line_cmd(t_prompt *prompt, char *content)
{
	free(prompt->line);
	if ((prompt->line = ft_strdup(content)) == NULL)
		return (-1);
	if (clear_all(prompt) == -1)
		return (-1);
	prompt->pos = ft_strlen(prompt->line);
	return (write_line(prompt));
}

int				check_complete(t_prompt *prompt, t_list *files)
{
	char	*line;
	size_t	length;

	if (ft_strchr(prompt->line, ' ') == NULL)
		return (complete_line_cmd(prompt, files->content));
	if (ft_strrchr(ft_strrchr(prompt->line, ' '), '/') == NULL)
		length = ft_strlen(prompt->line) -
			ft_strlen(ft_strrchr(prompt->line, ' ')) + 1;
	else
		length = ft_strlen(prompt->line) -
			ft_strlen(ft_strrchr(ft_strrchr(prompt->line, ' '), '/')) + 1;
	if ((line = malloc(length + 1 + ft_strlen(files->content))) == NULL)
		return (-1);
	ft_strncpy(line, prompt->line, length);
	line[length] = '\0';
	free(prompt->line);
	if ((prompt->line = ft_strrealloc(line, files->content)) == NULL)
		return (-1);
	if (clear_all(prompt) == -1)
		return (-1);
	if (check_dir(files->content, prompt) == -1)
		return (-1);
	prompt->pos = ft_strlen(prompt->line);
	return (write_line(prompt));
}

static void		update_path(char *path)
{
	size_t	i;

	if (ft_strrchr(path, '/') != NULL)
	{
		i = ft_strlen(path);
		while (path[i] != '/')
			i -= 1;
		path[i + 1] = '\0';
	}
}

int				handle_tab(t_prompt *prompt, char **environ)
{
	int		ret;
	int		dir;
	char	*path;

	if (prompt->line == NULL || prompt->quoting
			|| isquoting(prompt->commands) || prompt->line[prompt->pos] != '\0')
		return (0);
	else if (ft_strchr(prompt->line, ' ') == NULL)
		return (complete_bin(prompt, environ));
	else
	{
		if ((path = ft_strdup(ft_strrchr(prompt->line, ' ') + 1)) == NULL)
			return (-1);
		update_path(path);
		if ((dir = is_dir(path)) == -1)
		{
			free(path);
			return (-1);
		}
		ret = complete_arg(prompt, dir && path[ft_strlen(path) - 1] == '/' ?
				path : ".");
		free(path);
		return (ret);
	}
}
