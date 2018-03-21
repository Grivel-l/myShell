/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_checkquotes.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 01:34:30 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 01:36:48 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_checkquotes(t_quote *quotes, char c)
{
	if (c == '\'' && !quotes->doubleq)
		quotes->simpleq = !quotes->simpleq;
	else if (c == '"' && !quotes->simpleq)
		quotes->doubleq = !quotes->doubleq;
}
