/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free_alloc.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 00:08:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/02 00:10:56 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void		free_everything(char ***environ, t_dlist **list,
		char ***args, t_list **commands)
{
	if (environ != NULL)
		ft_freetab(environ);
	if (args != NULL)
		ft_freetab(args);
	if (commands != NULL)
		ft_lstfree(commands);
	if (list != NULL)
		ft_dlstfree(list);
}
