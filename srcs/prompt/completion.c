/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   completion.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/05 12:10:20 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/05 12:10:21 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	complete_line_cmd(t_prompt *prompt, char *content)
{
	free(prompt->line);
	if ((prompt->line = ft_strdup(content)) == NULL)
		return (-1);
	if (clear_all(prompt) == -1)
		return (-1);
	prompt->pos = ft_strlen(prompt->line);
	return (write_line(prompt));
}

static int	is_dir(char *file)
{
	struct stat	stats;

	if (access(file, F_OK) == -1)
		return (0);
	if (stat(file, &stats) == -1)
		return (-1);
	return (S_ISDIR(stats.st_mode));
}

static int	check_dir(char *content, t_prompt *prompt)
{
	int		dir;

	if ((dir = is_dir(ft_strrchr(prompt->line, ' ') + 1)) == -1)
		return (-1);
	if (dir)
	{
		if (content[ft_strlen(content) - 1] != '/')
			return (ft_addchar(&(prompt->line), '/'));
	}
	return (0);
}

static int	complete_cmd(t_prompt *prompt, t_list *files, char *content)
{
	char	*line;
	size_t	length;

	while (files != NULL)
	{
		if (ft_strncmp(content, files->content, ft_strlen(content)) == 0)
		{
			if (ft_strchr(prompt->line, ' ') == NULL)
				return (complete_line_cmd(prompt, files->content));
			if (ft_strrchr(ft_strrchr(prompt->line, ' '), '/') == NULL)
				length = ft_strlen(prompt->line) - ft_strlen(ft_strrchr(prompt->line, ' ')) + 1;
			else
				length = ft_strlen(prompt->line) - ft_strlen(ft_strrchr(ft_strrchr(prompt->line, ' '), '/')) + 1;
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
		files = files->next;
	}
	return (0);
}

static void	free_printed(t_list *printed)
{
	t_list	*next;

	while (printed != NULL)
	{
		next = printed->next;
		free(printed);
		printed = next;
	}
}

static int	half_complete(t_prompt *prompt, t_list *printed, size_t index)
{
	char	c;
	t_list	*pointer;

	pointer = printed;
	while (printed != NULL)
	{
		if (index >= ft_strlen(printed->content))
		{
			free_printed(pointer);
			return (0);
		}
		c = ((char *)printed->content)[index];
		printed = printed->next;
		if (printed != NULL &&
	(index >= ft_strlen(printed->content) || ((char *)(printed->content))[index] != c))
		{
			free_printed(pointer);
			return (0);
		}
	}
	if (c == '\0')
		return (0);
	if (insert_char(&(prompt->line), c, &(prompt->pos)) == -1)
	{
		free_printed(pointer);
		return (-1);
	}
	return (half_complete(prompt, pointer, index + 1));
}

static int	print_match(t_list *files, char *content, size_t *index, t_list **match)
{
	t_list	*last;
	t_list	*pointer;

	last = NULL;
	pointer = *match;
	while (*match != NULL)
		*match = (*match)->next;
	while (files != NULL)
	{
		if (ft_strncmp(content, files->content, ft_strlen(content)) == 0)
		{
			if ((*match = malloc(sizeof(t_list))) == NULL)
			{
				ft_lstfree(&pointer);
				return (-1);
			}
			(*match)->content = files->content;
			(*match)->next = NULL;
			if (last != NULL)
				last->next = *match;
			if (pointer == NULL)
				pointer = *match;
			last = *match;
			if (*index > 0)
				ft_putstr("    ");
			ft_putstr(files->content);
			*index += 1;
		}
		files = files->next;
	}
	*match = pointer;
	return (0);
}

static size_t	get_match_nbr(t_list *files, char *content)
{
	size_t	match;

	match = 0;
	while (files != NULL)
	{
		if (ft_strncmp(content, files->content, ft_strlen(content)) == 0)
			match += 1;
		files = files->next;
	}
	return (match);
}

static int	complete_bin_matched(t_prompt *prompt, char **paths, size_t match)
{
	size_t	index;
	t_list	*files;
	t_list	*printed;
	char 	**pointer;

	if (match > 1)
		ft_putchar('\n');
	index = 0;
	printed = NULL;
	pointer = paths;
	while (*paths != NULL)
	{
		if ((files = ft_readdir(*paths, 1)) == NULL)
			return (-1);
		if (match > 1)
			if (print_match(files, prompt->line, &index, &printed) == -1)
				return (-1);
		if (match == 1)
			if (complete_cmd(prompt, files, prompt->line) == -1)
				return (-1);
		ft_lstfree(&files);
		paths += 1;
	}
	if (match > 1)
	{
		if (half_complete(prompt, printed, ft_strlen(prompt->line)) == -1)
			return (-1);
		ft_putchar('\n');
		ft_putstr("\033[01;32m$\033[0m ");
		ft_putstr(prompt->line);
	}
	ft_freetab(&pointer);
	return (0);
}

static int	complete_bin(t_prompt *prompt, char **environ)
{
	size_t	match;
	char	*path;
	t_list	*files;
	char	**paths;
	char	**pointer;

	if ((path = get_myenv("PATH", environ)) == NULL)
		return (0);
	if ((paths = ft_strsplit(path, ':')) == NULL)
		return (-1);
	match = 0;
	pointer = paths;
	while (*pointer)
	{
		if ((files = ft_readdir(*pointer, 1)) == NULL)
			return (-1);
		match += get_match_nbr(files, prompt->line);
		ft_lstfree(&files);
		pointer += 1;
	}
	return (complete_bin_matched(prompt, paths, match));
}

static int	complete_arg(t_prompt *prompt, char *path)
{
	size_t	index;
	size_t	match;
	t_list	*files;
	t_list	*printed;
	char	*content;

	if ((files = ft_readdir(path, 1)) == NULL)
		return (-1);
	index = 0;
	printed = NULL;
	content = ft_strrchr(prompt->line, ' ') + 1;
	content = ft_strrchr(content, '/') == NULL ? content : ft_strrchr(content, '/') + 1;
	match = get_match_nbr(files, content);
	if (match > 1)
	{
		ft_putchar('\n');
		if (print_match(files, content, &index, &printed) == -1)
			return (-1);
		if (half_complete(prompt, printed, ft_strlen(content)) == -1)
			return (-1);
		ft_putchar('\n');
		ft_putstr("\033[01;32m$\033[0m ");
		ft_putstr(prompt->line);
	}
	else
	{
		if (complete_cmd(prompt, files, content) == -1)
			return (-1);
	}
	ft_lstfree(&files);
	return (0);
}

int			handle_tab(t_prompt *prompt, char **environ)
{
	size_t	i;
	int		ret;
	int		dir;
	char	*path;

	if (prompt->line == NULL || prompt->quoting || isquoting(prompt->commands))
		return (0);
	else if (ft_strchr(prompt->line, ' ') == NULL)
		return (complete_bin(prompt, environ));
	else
	{
		if ((path = ft_strdup(ft_strrchr(prompt->line, ' ') + 1)) == NULL)
			return (-1);
		if (ft_strrchr(path, '/') != NULL)
		{
			i = ft_strlen(path);
			while (path[i] != '/')
				i -= 1;
			path[i + 1] = '\0';
		}
		if ((dir = is_dir(path)) == -1)
			return (-1);
		ret = complete_arg(prompt, dir && path[ft_strlen(path) - 1] == '/' ? path : ".");
		free(path);
		return (ret);
	}
}
