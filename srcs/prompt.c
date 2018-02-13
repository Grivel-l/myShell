/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/14 00:53:39 by legrivel    ###    #+. /#+    ###.fr     */
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
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~(ECHO);
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	if (put_cap("im") == -1)
		return (-1);
	return (0);
}

int				wait_prompt(char **environ, t_ret cmd_ret, t_prompt *prompt)
{
	int			ret;

	ret = 0;
	prompt->pos = 0;
	prompt->line = NULL;
	ft_putstr("$ ");
	while (ret != 1)
	{
		if (read(STDIN_FILENO, prompt->buffer, 3) == -1)
			ret = -1;
		if ((ret = handle_input(prompt)) == -1)
			return (-1);
	}
	if (forward_cursor(prompt, ft_strlen(prompt->commands->content) - prompt->pos) == -1)
		return (-1);
	ft_putchar('\n');
	if (prompt->line != NULL)
		prompt->commands = prompt->commands->next;
	ft_strdel(&(prompt->line));
	return (wait_prompt(environ, cmd_ret, prompt));
}
