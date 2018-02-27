/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 19:24:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/27 09:03:24 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		main(int argc, char **argv, char **environ)
{
	t_command		cmd;
	t_prompt		prompt;
	struct termios	term;
	int				exit_status;

	(void)argv;
	if (argc != 1)
		return (1);
	if ((environ = ft_tabdup(environ)) == NULL)
		return (1);
	if (tcgetattr(0, &term) == -1)
		return (1);
	cmd.bin = NULL;
	cmd.exited = 0;
	cmd.cmd_ret = 0;
	cmd.args = NULL;
	cmd.environ = environ;
	cmd.fd[0] = -1;
	cmd.fd[1] = -1;
	cmd.fd2[0] = -1;
	cmd.fd2[1] = -1;
	prompt.extra = 0;
	prompt.quoting = 0;
	prompt.line = NULL;
	prompt.commands = NULL;
	prompt.copy_buffer = NULL;
	if (set_canonical(cmd.environ) == -1)
	{
		reset_term(term);
		free_everything(&cmd, NULL);
		return (1);
	}
	if (wait_prompt(&prompt, &cmd, term) == -1)
	{
		if (cmd.args != NULL)
			exit_status = cmd.args[1] == NULL ? WEXITSTATUS(cmd.cmd_ret) : ft_atoi(cmd.args[1]);
		else
			exit_status = cmd.cmd_ret;
		reset_term(term);
		free_everything(&cmd, &prompt);
		if (cmd.exited)
			exit(exit_status);
		return (1);
	}
	reset_term(term);
	free_everything(&cmd, &prompt);
	return (0);
}
