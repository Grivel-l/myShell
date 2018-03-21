/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_readdir.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/12/11 19:30:27 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 03:52:04 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static void	*quit(t_list *start, DIR *dir)
{
	ft_lstfree(&start);
	if (dir != NULL)
		closedir(dir);
	return (NULL);
}

static void	create_set(t_list **list, struct dirent *dir, t_list **start)
{
	*list = ft_lstnew(dir->d_name, dir->d_reclen);
	*start = *list;
}

t_list		*ft_readdir(char *filename, size_t get_hidden)
{
	DIR				*dir;
	t_list			*list;
	t_list			*start;
	struct dirent	*dir_content;

	if ((dir = opendir(filename)) == NULL)
		return (NULL);
	start = NULL;
	while ((dir_content = readdir(dir)) != NULL)
	{
		if ((get_hidden) || (!get_hidden && dir_content->d_name[0] != '.'))
		{
			if (start == NULL)
				create_set(&list, dir_content, &start);
			else
				ft_lstappend(&list,
					ft_lstnew(dir_content->d_name, dir_content->d_reclen));
			if (list == NULL)
				return (quit(start, dir));
		}
	}
	if (closedir(dir) == -1)
		return (quit(start, NULL));
	return (start == NULL ? ft_lstnew("", 0) : start);
}
