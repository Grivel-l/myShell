/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   complete2.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/20 21:10:54 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/20 21:34:20 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static t_list	*check_print(t_list *files, t_list **last,
		size_t *index, t_list **match)
{
	t_list	*pointer;

	if ((*match = ft_lstnew(files->content, ft_strlen(files->content))) == NULL)
		return (NULL);
	if (*last != NULL)
		(*last)->next = *match;
	pointer = *match;
	*last = *match;
	if (*index > 0)
		ft_putstr("    ");
	ft_putstr(files->content);
	*index += 1;
	return (pointer);
}

static int		end_bin_completion(t_prompt *prompt, t_list **printed,
		size_t match, char *pointer)
{
	if (pointer != NULL)
		return (-1);
	if (match > 1)
	{
		if (half_complete(prompt, *printed, ft_strlen(prompt->line)) == -1)
			return (-1);
		ft_putchar('\n');
		ft_putstr("\033[01;32m$\033[0m ");
		ft_putstr(prompt->line);
	}
	else
		ft_lstfree(printed);
	return (0);
}

static int		complete_bin_matched(t_prompt *prompt,
		char **paths, size_t match)
{
	size_t	index;
	t_list	*files;
	t_list	*printed;
	char	**pointer;

	if (match > 1)
		ft_putchar('\n');
	index = 0;
	printed = NULL;
	pointer = paths;
	while (*pointer != NULL)
	{
		if ((files = ft_readdir(*pointer, 1)) == NULL)
			break ;
		if (match > 1)
			if (print_match(files, prompt->line, &index, &printed) == -1)
				break ;
		if (match == 1)
			if (complete_cmd(prompt, files, prompt->line) == -1)
				break ;
		ft_lstfree(&files);
		pointer += 1;
	}
	ft_lstfree(&files);
	return (end_bin_completion(prompt, &printed, match, *pointer));
}

int				complete_bin(t_prompt *prompt, char **environ)
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
	if (complete_bin_matched(prompt, paths, match) == -1)
		return (-1);
	ft_freetab(&paths);
	return (0);
}

int				print_match(t_list *files, char *content,
		size_t *index, t_list **match)
{
	t_list	*last;
	t_list	*pointer;

	last = *match;
	pointer = *match;
	goto_lstend(match, &last);
	while (files != NULL)
	{
		if (ft_strncmp(content, files->content, ft_strlen(content)) == 0)
		{
			if (pointer == NULL)
				pointer = check_print(files, &last, index, match);
			else
			{
				if (check_print(files, &last, index, match) == NULL)
					return (-1);
			}
			if (pointer == NULL)
				return (-1);
		}
		files = files->next;
	}
	*match = pointer;
	return (0);
}
