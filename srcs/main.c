/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 19:24:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/24 21:41:44 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		main(int argc, char **argv, char **environ)
{
	t_command		cmd;
	struct termios	term;
	t_prompt		prompt;

	(void)argv;
	if (argc != 1)
		return (-1);
	if ((environ = ft_tabdup(environ)) == NULL)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	cmd.exited = 0;
	cmd.cmd_ret = 0;
	prompt.extra = 0;
	prompt.quoting = 0;
	prompt.commands = NULL;
	prompt.copy_buffer = NULL;
	if (set_canonical() == -1)
	{
		reset_term(term);
		free_everything(&environ, NULL);
		return (-1);
	}
	cmd.environ = environ;
	if (wait_prompt(&prompt, &cmd) == -1)
	{
		reset_term(term);
		free_everything(&(cmd.environ), &prompt);
		if (cmd.exited)
		{
			if (cmd.args[1] == NULL)
				exit(WEXITSTATUS(cmd.cmd_ret));
			else
				exit(ft_atoi(cmd.args[1]));
		}
		return (-1);
	}
	reset_term(term);
	free_everything(&(cmd.environ), &prompt);
	return (0);
}
