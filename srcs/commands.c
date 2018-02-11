/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   commands.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/09 19:10:36 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 15:55:22 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		isquoting(t_dlist *list)
{
	size_t	length;

	if (list == NULL)
		return (0);
	length = ft_strlen(list->content);
	if (length > 0 && list->content[length - 1] == '\n')
		return (1);
	return (0);
}

char	*get_previous_command(t_dlist *list)
{
	char	*str;

	if (list->previous != NULL && isquoting(list))
		return (get_previous_command(list->previous));
	if ((str = ft_strdup(list->content)) == NULL)
		return (NULL);
	list = list->next;
	while (list != NULL)
	{
		if ((str = ft_strrealloc(str, list->content)) == NULL)
		{
			free(str);
			return (NULL);
		}
		list = list->next;
	}
	return (str);
}
