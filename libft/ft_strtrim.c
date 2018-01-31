/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strtrim.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:21 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:21 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static char	*void_str(void)
{
	char	*str;

	if ((str = malloc(1)) == NULL)
		return (NULL);
	str[0] = '\0';
	return (str);
}

char		*ft_strtrim(char const *s)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	*str;
	size_t	length;

	i = 0;
	while (s != NULL && (s[i] == ' ' || (s[i] == '\t' || s[i] == '\n')))
		i += 1;
	length = s != NULL ? ft_strlen(s) : 0;
	if (length == 0 || length - i == 0)
		return (void_str());
	j = length - 1;
	while (s != NULL && (s[j] == ' ' || (s[j] == '\t' || s[j] == '\n')))
		j -= 1;
	j += 1;
	if (j != length)
		j -= i;
	if ((s == NULL) || ((str = malloc(j + 1)) == NULL))
		return (NULL);
	k = 0;
	while (k < j)
		str[k++] = s[i++];
	str[k] = '\0';
	return (str);
}
