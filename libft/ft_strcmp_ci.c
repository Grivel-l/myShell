/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strcmp.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:20 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:20 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

int		ft_strcmp_ci(const char *s1, const char *s2)
{
	unsigned char *str1;
	unsigned char *str2;
	unsigned char c1;
	unsigned char c2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (*str1 && *str2)
	{
		c1 = (*str1 >= 65 && *str1 <= 90) ? *str1 + 32 : *str1;
		c2 = (*str2 >= 65 && *str2 <= 90) ? *str2 + 32 : *str2;
		if (c1 != c2)
			return (c1 - c2);
		str1 += 1;
		str2 += 1;
	}
	return (((*str1 >= 65 && *str1 <= 90) ? *str1 + 32 : *str1) -
			((*str2 >= 65 && *str2 <= 90) ? *str2 + 32 : *str2));
}
