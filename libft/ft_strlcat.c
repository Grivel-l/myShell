/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strlcat.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:21 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:21 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char	*dest;
	char	*source;
	size_t	i;
	size_t	j;

	i = 0;
	dest = dst;
	source = (char *)src;
	while (i++ < size && *dest)
		dest += 1;
	j = dest - dst;
	i = size - j;
	if (i == 0)
		return (j + ft_strlen(source));
	while (*source)
	{
		if (i != 1)
		{
			*dest++ = *source;
			i -= 1;
		}
		source += 1;
	}
	*dest = '\0';
	return (j + (source - src));
}
