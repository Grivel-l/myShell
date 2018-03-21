/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 18:09:41 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int		read_stdin(t_prompt *prompt, char **environ)
{
	int		ret;

	ret = 0;
	while (ret != 1 && ret != 2)
	{
		if (read(STDIN_FILENO, prompt->buffer, 3) == -1)
			return (-1);
		if ((ret = handle_input(prompt, environ)) == -1)
			return (-1);
	}
	if (ret == 2 || (prompt->line == NULL && !isquoting(prompt->commands)))
		ft_putchar('\n');
	if (ret == 1 && prompt->line != NULL && ft_strochr(prompt->line, ' '))
	{
		ft_putchar('\n');
		ft_strdel(&prompt->line);
	}
	return (ret);
}

int				wait_prompt(t_prompt *prompt, t_command *cmd)
{
	int			ret;

	prompt->pos = 0;
	ft_putstr("\033[01;32m$\033[0m ");
	if (set_options(prompt->term, ICANON | ECHO | ISIG) == -1)
		return (-1);
	if ((ret = read_stdin(prompt, cmd->environ)) == -1)
		return (-1);
	if (ret == 2)
		return (0);
	if (prompt->line == NULL && !isquoting(prompt->commands))
		return (wait_prompt(prompt, cmd));
	if (goto_end(prompt) == -1)
		return (-1);
	ft_putchar('\n');
	cmd->bin = NULL;
	cmd->args = NULL;
	if (reset_term(prompt->term) == -1)
		return (-1);
	if ((ret = check_syntax(prompt)) == -1)
		return (-1);
	if (ret == 0 && prompt->line != NULL && treate_command(prompt, cmd) == -1)
		return (-1);
	ft_strdel(&(prompt->line));
	return (wait_prompt(prompt, cmd));
}
