/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   signals.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/23 02:01:33 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/23 02:21:11 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	handle_sigint(t_prompt *prompt)
{
	prompt->line = NULL;
	return (1);
}

static int	handle_eof(t_prompt *prompt)
{
	return (prompt->line == NULL ? 2 : 0);
}

int			handle_signals(t_prompt *prompt)
{
	if (prompt->buffer[0] == 3)
		return (handle_sigint(prompt));
	else if (prompt->buffer[0] == 4)
		return (handle_eof(prompt));
	return (0);
}
