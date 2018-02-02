/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_dlstfree.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 00:01:35 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/02 03:47:16 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstfree(t_dlist **list)
{
	while (*list != NULL && (*list)->previous != NULL)
		*list = (*list)->previous;
	while (*list != NULL)
	{
		if ((*list)->previous != NULL)
			free((*list)->previous);
		free((*list)->content);
		if ((*list)->next == NULL)
			free(*list);
		*list = (*list)->next;
	}
	*list = NULL;
}
