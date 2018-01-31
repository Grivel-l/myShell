/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_itoa.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:19 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static int	ft_abs(int n)
{
	return (n >= 0 ? n : n * -1);
}

static char	*zero(void)
{
	char	*str;

	if ((str = malloc(2)) == NULL)
		return (NULL);
	str[0] = '0';
	str[1] = '\0';
	return (str);
}

char		*ft_itoa(int n)
{
	size_t	i;
	char	*str;
	size_t	length;
	int		n_copy;

	if (n == 0)
		return (zero());
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
		str[length--] = ft_abs(n % 10) + 48;
		n /= 10;
	}
	str[length] = length == 0 ? '-' : str[length];
	return (str);
}
