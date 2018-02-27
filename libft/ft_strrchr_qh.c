/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strchr_qh.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 22:34:31 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/27 06:51:30 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static void	check_quotes(char c, t_quote *quotes)
{
	if (c == '"' && !quotes->simpleq)
		quotes->doubleq = !quotes->doubleq;
	else if (c == '\'' && !quotes->doubleq)
		quotes->simpleq = !quotes->simpleq;
}

char		*ft_strrchr_qh(char *str, char c)
{
	int		i;
	t_quote	quotes;

	i = ft_strlen(str);
	str += i - 1;
	quotes.simpleq = 0;
	quotes.doubleq = 0;
	while (i >= 0)
	{
		check_quotes(*str, &quotes);
		if (*str == c && !quotes.simpleq && !quotes.doubleq)
			return (str);
		i -= 1;
		str -= 1;
	}
	return (NULL);
}
