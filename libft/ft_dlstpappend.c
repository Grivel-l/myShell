/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_dlstnappend.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 23:38:43 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/01 23:51:00 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstpappend(t_dlist **list, t_dlist *link)
{
	t_dlist		*next;

	while ((*list)->previous != NULL)
		*list = (*list)->previous;
	next = *list;
	(*list)->previous = link;
	*list = (*list)->previous;
	(*list)->next = next;
}
