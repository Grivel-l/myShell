/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/26 15:41:47 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void			reset_term(struct termios term)
{
	put_cap("ei");
	tcsetattr(0, TCSANOW, &term);
}

int				set_canonical(char **environ)
{
	int				ret;
	char			*name;
	struct termios	term;

	if ((name = get_myenv("TERM", environ)) == NULL)
	{
		ft_putstr_fd("$TERM is missing", 2);
		return (-1);
	}
	ret = tgetent(NULL, name);
	if (ret == 1 && tcgetattr(0, &term) == -1)
		ret = -1;
	if (ret == -1 || ret == 0)
		return (-1);
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	if (put_cap("im") == -1)
		return (-1);
	return (0);
}

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
		if ((ret = handle_input(prompt)) == -1)
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
	while (prompt->pos < ft_strlen(prompt->line))
		if (right_arrow(prompt) == -1)
			return (-1);
	ft_putchar('\n');
	cmd->bin = NULL;
	cmd->args = NULL;
	set_options(term, ICANON | ECHO);
	if (prompt->line != NULL && treate_command(prompt, cmd) == -1)
		return (-1);
	ft_strdel(&(prompt->line));
	return (wait_prompt(prompt, cmd, term));
}
