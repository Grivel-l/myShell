/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   complete.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/20 21:08:37 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/20 21:34:10 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static void		init_values(t_prompt *prompt, t_list **printed,
		size_t *index, char **content)
{
	*index = 0;
	*printed = NULL;
	*content = ft_strrchr(prompt->line, ' ') + 1;
	*content = ft_strrchr(*content, '/') == NULL ?
		*content : ft_strrchr(*content, '/') + 1;
}

int				complete_cmd(t_prompt *prompt, t_list *files, char *content)
{
	while (files != NULL)
	{
		if (ft_strncmp(content, files->content, ft_strlen(content)) == 0)
			return (check_complete(prompt, files));
		files = files->next;
	}
	return (0);
}

int				half_complete(t_prompt *prompt, t_list *printed, size_t index)
{
	char	c;
	t_list	*pointer;

	pointer = printed;
	while (printed != NULL)
	{
		if (index >= ft_strlen(printed->content))
			return (free_printed(&pointer, 0));
		c = ((char *)printed->content)[index];
		printed = printed->next;
		if (printed != NULL &&
				(index >= ft_strlen(printed->content) ||
				((char *)(printed->content))[index] != c))
			return (free_printed(&pointer, 0));
	}
	if (c == '\0')
		return (free_printed(&pointer, 0));
	if (insert_char(&(prompt->line), c, &(prompt->pos)) == -1)
		return (free_printed(&pointer, -1));
	return (half_complete(prompt, pointer, index + 1));
}

static int		print_and_match(t_prompt *prompt,
		t_list *printed, size_t length)
{
	if (half_complete(prompt, printed, length) == -1)
		return (-1);
	ft_putstr("\n\033[01;32m$\033[0m ");
	ft_putstr(prompt->line);
	return (0);
}

int				complete_arg(t_prompt *prompt, char *path)
{
	size_t	index;
	size_t	match;
	t_list	*files;
	t_list	*printed;
	char	*content;

	if ((files = ft_readdir(path, 1)) == NULL)
		return (-1);
	init_values(prompt, &printed, &index, &content);
	match = get_match_nbr(files, content);
	if (match > 1)
	{
		ft_putchar('\n');
		if (print_match(files, content, &index, &printed) == -1)
			return (free_printed(&files, -1));
		if (print_and_match(prompt, printed, ft_strlen(content)) == -1)
			return (free_printed(&files, -1));
	}
	else
	{
		if (complete_cmd(prompt, files, content) == -1)
			return (free_printed(&files, -1));
	}
	ft_lstfree(&files);
	return (0);
}
