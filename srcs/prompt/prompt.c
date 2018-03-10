/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/28 02:04:55 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int				wait_prompt(t_prompt *prompt, t_command *cmd, struct termios term)
{
	int			ret;

	ret = 0;
	prompt->pos = 0;
	ft_putstr("\033[01;32m$\033[0m ");
	set_options(term, ICANON | ECHO | ISIG);
	while (ret != 1 && ret != 2)
	{
		if (read(STDIN_FILENO, prompt->buffer, 3) == -1)
			return (-1);
		if ((ret = handle_input(prompt, cmd->environ)) == -1)
			return (-1);
	}
	if (ret == 2)
	{
		ft_putchar('\n');
		return (0);
	}
	if (prompt->line == NULL && !isquoting(prompt->commands))
	{
		ft_putchar('\n');
		return (wait_prompt(prompt, cmd, term));
	}
	if (goto_end(prompt) == -1)
		return (-1);
	ft_putchar('\n');
	cmd->bin = NULL;
	cmd->args = NULL;
	set_options(term, ICANON | ECHO);
	if ((ret = check_syntax(prompt)) == -1)
		return (-1);
	if (ret == 0 && prompt->line != NULL && treate_command(prompt, cmd) == -1)
		return (-1);
	ft_strdel(&(prompt->line));
	return (wait_prompt(prompt, cmd, term));
}
