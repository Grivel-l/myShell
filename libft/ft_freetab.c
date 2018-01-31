/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_freetab.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 19:50:14 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/30 22:58:22 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_freetab(char ***tab)
{
	char	**tab_pointer;

	tab_pointer = *tab;
	while (**tab != NULL)
	{
		ft_strdel(*tab);
		*tab += 1;
	}
	free(tab_pointer);
	*tab = NULL;
}
