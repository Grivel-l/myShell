/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free_alloc.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 00:08:02 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/26 17:20:27 by legrivel    ###    #+. /#+    ###.fr     */
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
	close_fd(cmd->fd);
}

int			free_and_return(char *str, int return_value)
{
	free(str);
	return (return_value);
}

int			free_and_return2(char *str, t_list **pointer, int return_value)
{
	free(str);
	ft_lstfree(pointer);
	return (return_value);
}
