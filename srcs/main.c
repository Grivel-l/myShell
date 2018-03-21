/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 19:24:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 02:48:17 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	init_values(t_prompt *prompt, t_command *cmd,
			char ***environ, struct termios *term)
{
	if (tcgetattr(0, term) == -1)
		return (1);
	if ((*environ = ft_tabdup(*environ)) == NULL)
		return (1);
	cmd->bin = NULL;
	cmd->exited = 0;
	cmd->cmd_ret = 0;
	cmd->args = NULL;
	cmd->environ = *environ;
	cmd->fd[0] = -1;
	cmd->fd[1] = -1;
	prompt->extra = 0;
	prompt->quoting = 0;
	prompt->term = *term;
	prompt->line = NULL;
	prompt->commands = NULL;
	prompt->copy_buffer = NULL;
	return (0);
}

static int	quit_program(t_command *cmd, t_prompt *prompt,
			struct termios term, int return_value)
{
	int		ret;

	ret = reset_term(term);
	free_everything(cmd, prompt);
	return (ret == -1 ? 1 : return_value);
}

int			main(int argc, char **argv, char **environ)
{
	t_command		cmd;
	t_prompt		prompt;
	struct termios	term;
	int				exit_status;

	(void)argv;
	if (argc != 1 || init_values(&prompt, &cmd, &environ, &term) == -1)
		return (1);
	if (set_canonical(cmd.environ) == -1)
		return (quit_program(&cmd, NULL, term, 1));
	if (wait_prompt(&prompt, &cmd) == -1)
	{
		if (cmd.args != NULL)
			exit_status = cmd.args[1] == NULL ?
				cmd.cmd_ret : ft_atoi(cmd.args[1]);
		else
			exit_status = cmd.cmd_ret;
		reset_term(term);
		free_everything(&cmd, &prompt);
		if (cmd.exited)
			exit(exit_status);
		return (1);
	}
	return (quit_program(&cmd, &prompt, term, 0));
}
