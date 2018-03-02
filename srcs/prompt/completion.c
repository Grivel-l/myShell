/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   completion.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/28 02:04:55 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	complete_cmd(t_prompt *prompt, t_list *files, char *content)
{
	char	*line;
	size_t	length;

	while (files != NULL)
	{
		if (ft_strncmp(content, files->content, ft_strlen(content)) == 0)
		{
			length = ft_strlen(prompt->line) -
		ft_strlen(ft_strrchr(prompt->line, ' ')) + 1;
			if ((line = malloc(length + 1 + ft_strlen(files->content))) == NULL)
				return (-1);
			ft_strncpy(line, prompt->line, length);
			line[length] = '\0';
			free(prompt->line);
			if ((prompt->line = ft_strrealloc(line, files->content)) == NULL)
				return (-1);
			if (clear_all(prompt) == -1)
				return (-1);
			prompt->pos = ft_strlen(prompt->line);
			return (write_line(prompt));
		}
		files = files->next;
	}
	return (0);
}

static int	print_match(t_list *files, char *content, size_t *index)
{
	while (files != NULL)
	{
		if (ft_strncmp(content, files->content, ft_strlen(content)) == 0)
		{
			if (*index > 0)
				ft_putstr("    ");
			ft_putstr(files->content);
			*index += 1;
		}
		files = files->next;
	}
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
	char 	**pointer;

	if (match > 1)
		ft_putchar('\n');
	index = 0;
	pointer = paths;
	while (*paths != NULL)
	{
		if ((files = ft_readdir(*paths, 1)) == NULL)
			return (-1);
		if (match > 1)
			if (print_match(files, prompt->line, &index) == -1)
				return (-1);
		ft_lstfree(&files);
		if (match == 1)
		{
			if (complete_cmd(prompt, files, prompt->line) == -1)
				return (-1);
			ft_lstfree(&files);
			break ;
		}
		paths += 1;
	}
	ft_freetab(&pointer);
	if (match > 1)
	{
		ft_putchar('\n');
		ft_putstr("\033[01;32m$\033[0m ");
		ft_putstr(prompt->line);
	}
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

static int	complete_arg(t_prompt *prompt)
{
	size_t	index;
	size_t	match;
	t_list	*files;
	char	*content;

	index = 0;
	content = ft_strrchr(prompt->line, ' ') + 1;
	if ((files = ft_readdir(".", 1)) == NULL)
		return (-1);
	match = get_match_nbr(files, content);
	if (match > 1)
	{
		ft_putchar('\n');
		if (print_match(files, content, &index) == -1)
			return (-1);
		ft_putchar('\n');
		ft_putstr("\033[01;32m$\033[0m ");
		ft_putstr(prompt->line);
	}
	else
		if (complete_cmd(prompt, files, content) == -1)
			return (-1);
	ft_lstfree(&files);
	return (0);
}

int			handle_tab(t_prompt *prompt, char **environ)
{
	if (prompt->line == NULL || prompt->quoting || isquoting(prompt->commands))
		return (0);
	else if (ft_strchr(prompt->line, ' ') == NULL)
		return (complete_bin(prompt, environ));
	else
		return (complete_arg(prompt));
}
