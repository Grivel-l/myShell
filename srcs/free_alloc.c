/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free_alloc.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 00:08:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/14 00:33:42 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void		free_everything(char ***environ, t_prompt *prompt)
{
	if (environ != NULL)
		ft_freetab(environ);
	if (prompt != NULL)
	{
		ft_strdel(&(prompt->line));
		ft_strdel(&(prompt->copy_buffer));
		ft_dlstfree(&(prompt->commands));
	}
}
