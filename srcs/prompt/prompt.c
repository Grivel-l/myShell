/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/23 23:24:32 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void			reset_term(struct termios term)
{
	tcsetattr(0, TCSANOW, &term);
}

int				set_canonical(void)
{
	int				ret;
	char			*name;
	struct termios	term;

	if ((name = getenv("TERM")) == NULL)
	{
		ft_putstr_fd("$TERM is missing", 2);
		return (-1);
	}
	ret = tgetent(NULL, name);
	if (ret == 1 && tcgetattr(0, &term) == -1)
		ret = -1;
	if (ret == -1 || ret == 0)
		return (-1);
	term.c_lflag &= ~(ICANON | ECHO | ISIG);
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	if (put_cap("im") == -1)
		return (-1);
	return (0);
}

int				wait_prompt(char **environ, t_ret cmd_ret, t_prompt *prompt)
{
	int			ret;
	t_command	command;

	ret = 0;
	prompt->pos = 0;
	prompt->line = NULL;
	ft_putstr("\033[01;32m$\033[0m ");
	while (ret != 1 && ret != 2)
	{
		if (read(STDIN_FILENO, prompt->buffer, 3) == -1)
			ret = -1;
		if ((ret = handle_input(prompt)) == -1)
			return (-1);
	}
	ft_putchar('\n');
	if (ret == 2)
	{
		ft_strdel(&(prompt->line));
		return (0);
	}
	if (prompt->line == NULL && !isquoting(prompt->commands))
		return (wait_prompt(environ, cmd_ret, prompt));
	if (forward_cursor(prompt, ft_strlen(prompt->commands->content) - prompt->pos) == -1)
	{
		ft_strdel(&(prompt->line));
		return (-1);
	}
	command.bin = NULL;
	command.args = NULL;
	command.environ = environ;
	if (prompt->line != NULL && treate_command(prompt, &command) == -1)
	{
		ft_strdel(&(prompt->line));
		return (-1);
	}
	ft_strdel(&(prompt->line));
	return (wait_prompt(environ, cmd_ret, prompt));
}
