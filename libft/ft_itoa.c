/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_itoa.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/16 22:10:34 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char		*ft_itoa(int n)
{
	size_t	i;
	char	*str;
	size_t	length;
	int		n_copy;

	if (n == 0)
		return (ft_strdup("0"));
	i = 0;
	n_copy = n;
	while (n_copy != 0)
	{
		n_copy /= 10;
		i += 1;
	}
	length = n < 0 ? i + 2 : i + 1;
	if ((str = malloc(length--)) == NULL)
		return (NULL);
	str[length--] = '\0';
	while (n != 0)
	{
		str[length--] = ((n % 10) >= 0 ? n % 10 : n % 10 * -1) + 48;
		n /= 10;
	}
	str[length] = length == 0 ? '-' : str[length];
	return (str);
}
