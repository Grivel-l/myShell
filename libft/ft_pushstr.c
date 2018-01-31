/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_pushstr.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/12/11 18:01:59 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/30 22:48:47 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static int	complete(char ***tab, char **new,
	char **tab_pointer, char **pointer)
{
	new += 1;
	*new = NULL;
	free(tab_pointer);
	*tab = pointer;
	return (0);
}

int			ft_pushstr(char ***tab, char *str)
{
	char	**new;
	char	**pointer;
	char	**tab_pointer;

	if ((new = malloc(sizeof(char *) * (ft_tablen(*tab) + 2))) == NULL)
		return (-1);
	pointer = new;
	tab_pointer = *tab;
	while (**tab != NULL)
	{
		if ((*new++ = ft_strdup(**tab)) == NULL)
		{
			ft_freetab(&pointer);
			return (-1);
		}
		free(**tab);
		*tab += 1;
	}
	if ((*new = malloc(ft_strlen(str) + 1)) == NULL)
	{
		ft_freetab(&pointer);
		return (-1);
	}
	ft_strcpy(*new, str);
	return (complete(tab, new, tab_pointer, pointer));
}
