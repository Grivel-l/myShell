/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstfree.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/30 00:11:49 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/03 20:05:56 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	ft_lstfree(t_list **list)
{
	t_list	*pointer;

	while ((*list) != NULL)
	{
		pointer = *list;
		ft_strdel((char **)&((*list)->content));
		*list = (*list)->next;
		free(pointer);
	}
	*list = NULL;
}
