/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:21 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:21 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	*pointer;

	if (needle[0] == '\0')
		return ((char *)haystack);
	i = 0;
	pointer = NULL;
	while (haystack[i])
	{
		j = 0;
		k = i;
		pointer = (char *)&haystack[i];
		while (needle[j])
		{
			if (needle[j] != haystack[k++])
				break ;
			if (needle[j++ + 1] == '\0')
				return (pointer);
		}
		pointer = NULL;
		i += 1;
	}
	return (pointer);
}
