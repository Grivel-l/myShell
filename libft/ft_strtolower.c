/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strtolower.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/14 23:14:15 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/14 23:14:16 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtolower(char *str)
{
	char	*str_pointer;

	str_pointer = str;
	while (*str)
	{
		if (*str >= 65 && *str <= 90)
			*str += 32;
		str += 1;
	}
	return (str_pointer);
}
