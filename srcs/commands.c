/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   commands.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/09 19:10:36 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 17:22:34 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		isquoting(t_dlist *list)
{
	size_t	length;

	if (list == NULL)
		return (0);
	length = ft_strlen(list->content);
	if (length > 0 && list->content[length - 1] == '\n')
		return (1);
	return (0);
}
