/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/20 01:51:29 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/20 03:33:19 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

size_t		get_match_nbr(t_list *files, char *content)
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

int			free_printed(t_list *printed, int ret)
{
	ft_lstfree(&printed);
	return (ret);
}

int			is_dir(char *file)
{
	struct stat	stats;

	if (access(file, F_OK) == -1)
		return (0);
	if (stat(file, &stats) == -1)
		return (-1);
	return (S_ISDIR(stats.st_mode));
}

int			check_dir(char *content, t_prompt *prompt)
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
