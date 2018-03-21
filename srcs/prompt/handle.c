/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   handle.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 01:19:51 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 01:25:36 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	handle_backspace(t_prompt *prompt)
{
	if (prompt->pos == 0)
		return (0);
	if (clear_all(prompt) == -1)
		return (-1);
	if (remove_char(&(prompt->line), &(prompt->pos)) == -1)
		return (-1);
	return (write_line(prompt));
}

static int	handle_basics(t_prompt *prompt)
{
	char	buffer[1];

	buffer[0] = prompt->buffer[0];
	if (buffer[0] == 127)
		return (handle_backspace(prompt));
	else if (buffer[0] == 10)
		return (handle_return(prompt));
	else if (buffer[0] >= 32 && buffer[0] <= 126)
		return (handle_printable(prompt, buffer[0]));
	return (0);
}

int			handle_printable(t_prompt *prompt, char c)
{
	if (clear_all(prompt) == -1)
		return (-1);
	if (insert_char(&(prompt->line), c, &(prompt->pos)) == -1)
		return (-1);
	return (write_line(prompt));
}

int			handle_input(t_prompt *prompt, char **environ)
{
	char	c;

	c = prompt->buffer[0];
	if ((c >= 32 && c <= 127) || c == 10)
		return (handle_basics(prompt));
	else if (c == 6 || c == 7 || c == 8)
		return (handle_ccp(prompt));
	else if (c == 18 || c == 20 || c == 23 || c == 5 || c == 27)
		return (handle_movements(prompt));
	else if (c == 3 || c == 4)
		return (handle_signals(prompt));
	else if (c == 9)
		return (handle_tab(prompt, environ));
	return (0);
}
