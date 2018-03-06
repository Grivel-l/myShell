/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_addchar.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/06 01:29:01 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/06 01:31:37 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

int		ft_addchar(char **str, char c)
{
	size_t	length;
	char	*result;

	length = *str == NULL ? 0 : ft_strlen(*str);
	if ((result = malloc(length + 2)) == NULL)
		return (-1);
	if (*str != NULL)
		ft_strcpy(result, *str);
	result[length] = c;
	result[length + 1] = '\0';
	if (*str != NULL)
		free(*str);
	*str = result;
	return (0);
}
