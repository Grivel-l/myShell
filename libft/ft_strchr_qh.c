/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strchr_qh.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 22:34:31 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/01 22:47:16 by legrivel    ###    #+. /#+    ###.fr     */
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

char	*ft_strchr_qh(char *str, char c)
{
	t_quote	quotes;

	quotes.simpleq = 0;
	quotes.doubleq = 0;
	while (*str)
	{
		check_quotes(*str, &quotes);
		if (*str == c && !quotes.simpleq && !quotes.doubleq)
			return (str);
		str += 1;
	}
	return (NULL);
}
