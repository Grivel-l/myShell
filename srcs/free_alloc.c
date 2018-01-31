/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free_alloc.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 00:08:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/30 22:52:34 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

void		free_everything(char ***environ, t_list **path_env,
		char ***args, t_list **commands)
{
	if (environ != NULL)
		ft_freetab(environ);
	if (path_env != NULL)
		ft_lstfree(path_env);
	if (args != NULL)
		ft_freetab(args);
	if (commands != NULL)
		ft_lstfree(commands);
}
