/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strtrim.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:21 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/18 01:58:21 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strtrim(char *s)
{
	size_t	i;
	char	*str;

	while ((*s == ' ' || *s == '\t') && *s != '\0')
		s += 1;
	if (*s == '\0')
		return (ft_strnew(1));
	i = ft_strlen(s) - 1;
	while (s[i] == ' ' || s[i] == '\t')
		i -= 1;
	if ((str = malloc(i + 2)) == NULL)
		return (NULL);
	ft_strncpy(str, s, i + 1);
	str[i + 1] = '\0';
	return (str);
}
