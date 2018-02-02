/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/02 17:29:58 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

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
	if (ret == -1)
	{
		free(name);
		return (-1);
	}
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~(ECHO);
	if (tcsetattr(0, TCSANOW, &term) == -1)
	{
		free(name);
		return (-1);
	}
	if (put_cpblt("im") == -1)
		return (-1);
	return (0);
}

int				wait_prompt(char **environ, t_ret cmd_ret, t_dlist **list)
{
	int		ret;
	char	*line;
	char	buffer[3];

	ret = 0;
	line = NULL;
	ft_putstr("$ ");
	while (1)
	{
		if (read(STDIN_FILENO, buffer, 3) == -1)
			ret = -1;
		if (ret == 0 && handle_input(buffer) == -1)
			ret = -1;
		if (ret == 0 && line != NULL)
			if ((line = ft_strrealloc(line, buffer)) == NULL)
				ret = -1;
		if (ret == 0 && line == NULL)
			if ((line = ft_strdup(buffer)) == NULL)
				ret = -1;
		ft_strdel(&line);
		if (ret == -1)
			return (-1);
	}
	return (wait_prompt(environ, cmd_ret, list));
}
