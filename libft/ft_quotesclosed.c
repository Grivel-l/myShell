/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_quotesclosed.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/09 16:53:29 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/09 16:58:56 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

int		ft_quotesclosed(char *str)
{
	t_quote	quotes;

	quotes.simpleq = 0;
	quotes.doubleq = 0;
	while (*str)
	{
		if (*str == '\'' && !quotes.doubleq)
			quotes.simpleq = !quotes.simpleq;
		else if (*str == '"' && !quotes.simpleq)
			quotes.doubleq = !quotes.doubleq;
		str += 1;
	}
	return (quotes.simpleq == 0 && quotes.doubleq == 0 ? 1 : 0);
}
