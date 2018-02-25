/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_dlstfree.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 00:01:35 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/17 01:54:24 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstfree(t_dlist **list)
{
	t_dlist	*next;

	while (*list != NULL && (*list)->previous != NULL)
		*list = (*list)->previous;
	while (*list != NULL)
	{
		free((*list)->content);
		if ((*list)->next == NULL)
		{
			free(*list);
			*list = NULL;
		}
		else
		{
			next = (*list)->next;
			free(*list);
			*list = next;
		}
	}
}
