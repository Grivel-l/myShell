/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_putdlst.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 23:41:30 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/01 23:48:29 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_putdlst(t_dlist *list)
{
	while (list != NULL)
	{
		ft_putstr(list->content);
		ft_putchar('\n');
		list = list->next;
	}
}
