/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_dlstnappend.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 23:38:43 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/01 23:50:12 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstnappend(t_dlist **list, t_dlist *link)
{
	t_dlist		*previous;

	while ((*list)->next != NULL)
		*list = (*list)->next;
	previous = *list;
	(*list)->next = link;
	*list = (*list)->next;
	(*list)->previous = previous;
}
