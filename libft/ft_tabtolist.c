/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_tabtolist.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/30 00:05:41 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/30 00:25:21 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

t_list	*ft_tabtolist(char **tab)
{
	t_list	*list;
	t_list	*pointer;

	list = NULL;
	pointer = NULL;
	while (*tab != NULL)
	{
		if (list != NULL)
		{
			ft_lstappend(&list, ft_lstnew(*tab, ft_strlen(*tab)));
			if (list == NULL)
			{
				ft_lstfree(&pointer);
				return (NULL);
			}
		}
		if (list == NULL)
			if ((list = ft_lstnew(*tab, ft_strlen(*tab))) == NULL)
				return (NULL);
		if (pointer == NULL)
			pointer = list;
		tab += 1;
	}
	return (pointer);
}
