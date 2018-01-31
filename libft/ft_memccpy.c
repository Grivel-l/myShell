/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memccpy.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:19 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	*result;
	unsigned char	*source;

	if (n == 0)
		return (NULL);
	result = dst;
	source = (void *)src;
	while (n-- > 0)
	{
		*result++ = *source++;
		source -= 1;
		if (*source++ == (unsigned char)c)
			break ;
	}
	source -= 1;
	if (*source != (unsigned char)c)
		return (NULL);
	return (result);
}
