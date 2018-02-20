/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_trim.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/20 22:47:49 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/20 22:57:00 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

int		ft_trim(char **str)
{
	char	*dup;
	size_t	length;
	char	*pointer;

	pointer = *str;
	while (**str == ' ')
		*str += 1;
	length = ft_strlen(*str);
	while ((*str)[length - 1] == ' ')
		length -= 1;
	if ((dup = ft_strnew(length + 1)) == NULL)
	{
		ft_strdel(&pointer);
		return (-1);
	}
	ft_strncpy(dup, *str, length);
	*str = dup;
	free(pointer);
	return (0);
}
