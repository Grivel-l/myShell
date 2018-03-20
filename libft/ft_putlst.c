/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_putlst.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/30 00:02:23 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/20 03:08:20 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_putlst(t_list *list)
{
	while (list != NULL)
	{
		ft_putstr(list->content);
		ft_putchar('\n');
		list = list->next;
	}
}
