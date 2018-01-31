/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstnew.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/28 01:07:05 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	char	*str;
	char	*copy;
	t_list	*link;

	if ((link = malloc(sizeof(t_list))) == NULL)
		return (NULL);
	if ((str = malloc(content_size + 1)) == NULL)
		return (NULL);
	if (content != NULL)
	{
		link->content = str;
		copy = (char *)content;
		while (*copy)
			*str++ = *copy++;
		*str = '\0';
		link->content_size = content_size;
	}
	else
	{
		link->content_size = 0;
		link->content = NULL;
		ft_strdel(&str);
	}
	link->next = NULL;
	return (link);
}
