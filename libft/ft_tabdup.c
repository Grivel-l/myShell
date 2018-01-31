/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_tabdup.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 19:47:00 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/30 22:49:01 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	**ft_tabdup(char **tab)
{
	char	**copy;
	char	**tab_pointer;
	char	**copy_pointer;

	if ((copy = malloc(sizeof(char *) * (ft_tablen(tab) + 1))) == NULL)
		return (NULL);
	*copy = NULL;
	tab_pointer = tab;
	copy_pointer = copy;
	while (*tab != NULL)
	{
		if ((*copy = ft_strdup(*tab)) == NULL)
		{
			ft_freetab(&copy);
			return (NULL);
		}
		tab += 1;
		copy += 1;
	}
	*copy = NULL;
	tab = tab_pointer;
	return (copy_pointer);
}
