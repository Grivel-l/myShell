/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   signals.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/23 02:01:33 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/28 00:45:20 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	handle_sigint(t_prompt *prompt)
{
	t_dlist	*previous;
	if (isquoting(prompt->commands))
	{
		previous = prompt->commands->previous;
		ft_strdel(&(prompt->commands->content));
		free(prompt->commands);
		prompt->commands = previous;
		if (prompt->commands != NULL)
			prompt->commands->next = NULL;
	}
	if (goto_end(prompt) == -1)
		return (-1);
	ft_strdel(&(prompt->line));
	return (1);
}

static int	handle_eof(t_prompt *prompt)
{
	if (isquoting(prompt->commands))
		return (1);
	return (prompt->line == NULL || prompt->line[0] == '\0' ? 2 : 0);
}

int			set_options(struct termios term, int options)
{
	term.c_lflag &= ~(options);
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	return (0);
}

int			handle_signals(t_prompt *prompt)
{
	if (prompt->buffer[0] == 3)
		return (handle_sigint(prompt));
	else if (prompt->buffer[0] == 4)
		return (handle_eof(prompt));
	return (0);
}
