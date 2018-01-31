/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strsplit.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:21 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:21 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_tab_length(char const *s, char c)
{
	size_t	i;
	size_t	j;

	if (s == NULL)
		return (0);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i += 1;
		while (s[i] != c && s[i])
			i += 1;
		j += 1;
	}
	return (j);
}

static void		*allocate_tab(char const *s, char c, char **tab)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	k = 0;
	while (s[i])
	{
		j = 0;
		while (s[i] && s[i] == c)
			i += 1;
		while (s[i] && s[i++] != c)
			j += 1;
		if (j > 0 && (tab[k++] = malloc(j + 1)) == NULL)
			return (NULL);
	}
	if (s[0] == '\0')
	{
		if ((tab[k] = malloc(1)) == NULL)
			return (NULL);
		tab[k++][0] = '\0';
	}
	tab[k] = NULL;
	return (tab[0]);
}

static int		is_cutable(char *s, char c)
{
	while (*s)
	{
		if (*s++ != c)
			return (1);
	}
	return (0);
}

static char		**manage_short_tab(char **tab, char const *s, char c)
{
	size_t	i;
	size_t	j;

	if (!is_cutable((char *)s, c))
	{
		if ((tab = malloc(sizeof(char *))) == NULL)
			return (NULL);
		tab[0] = NULL;
		return (tab);
	}
	i = 0;
	while (s[i] == c)
		i += 1;
	j = 0;
	while (s[i] != c && s[i])
	{
		i += 1;
		j += 1;
	}
	if ((tab = malloc(sizeof(char *) * 2)) == NULL ||
			(tab[0] = malloc(j + 1)) == NULL)
		return (NULL);
	ft_strcpy(tab[0], &s[i - j]);
	tab[1] = NULL;
	return (tab);
}

char			**ft_strsplit(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	**tab;

	tab = NULL;
	if (get_tab_length(s, c) == 1)
		return (manage_short_tab(tab, s, c));
	if (s == NULL || (tab = malloc(sizeof(char *) * (get_tab_length(s, c) + 1)\
)) == NULL || (allocate_tab(s, c, tab) == NULL))
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i += 1;
		k = 0;
		while (s[i] != c && s[i])
			tab[j][k++] = s[i++];
		if (k > 0)
			tab[j++][k] = '\0';
	}
	tab[j] = NULL;
	return (tab);
}
