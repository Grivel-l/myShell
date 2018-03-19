/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   signals.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/23 02:01:33 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/20 00:49:25 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	handle_sigint(t_prompt *prompt)
{
	if (isquoting(prompt->commands))
	{
		free(prompt->commands->content);
		if ((prompt->commands->content = ft_strnew(1)) == NULL)
			return (-1);
	}
	if (goto_end(prompt) == -1)
		return (-1);
	ft_strdel(&(prompt->line));
	return (1);
}

static int	handle_eof(t_prompt *prompt)
{
	if (isquoting(prompt->commands))
		return (handle_sigint(prompt));
	return (prompt->line == NULL || prompt->line[0] == '\0' ? 2 : 0);
}

int			handle_signals(t_prompt *prompt)
{
	if (prompt->buffer[0] == 3)
		return (handle_sigint(prompt));
	else if (prompt->buffer[0] == 4)
		return (handle_eof(prompt));
	return (0);
}
