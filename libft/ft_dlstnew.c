/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_dlstnew.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 23:23:01 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/01 23:46:25 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

t_dlist		*ft_dlstnew(char *content)
{
	t_dlist	*list;

	if ((list = malloc(sizeof(t_dlist))) == NULL)
		return (NULL);
	if ((list->content = ft_strdup(content)) == NULL)
	{
		free(list);
		return (NULL);
	}
	list->next = NULL;
	list->previous = NULL;
	return (list);
}
