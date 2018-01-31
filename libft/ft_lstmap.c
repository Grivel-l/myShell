/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstmap.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:19 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*pointer;
	t_list	*result;
	t_list	*previous;

	pointer = lst;
	while (lst != NULL)
		lst = lst->next;
	lst = pointer;
	pointer = NULL;
	previous = NULL;
	while (lst != NULL)
	{
		if ((result = ft_lstnew(lst->content, lst->content_size)) == NULL)
			return (NULL);
		result = f(result);
		if (pointer == NULL)
			pointer = result;
		if (previous != NULL)
			previous->next = result;
		previous = result;
		lst = lst->next;
	}
	return (pointer);
}
