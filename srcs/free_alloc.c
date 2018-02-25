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

void		free_everything(t_command *cmd, t_prompt *prompt)
{
	if (prompt != NULL)
	{
		ft_strdel(&(prompt->line));
		ft_strdel(&(prompt->copy_buffer));
		ft_dlstfree(&(prompt->commands));
	}
	ft_strdel(&(cmd->bin));
	ft_freetab(&(cmd->args));
	ft_freetab(&(cmd->environ));
	close_all_fd(cmd->fildes);
}
