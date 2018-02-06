/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/06 21:01:40 by legrivel    ###    #+. /#+    ###.fr     */
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
	if (put_cap("im") == -1)
		return (-1);
	return (0);
}

int				wait_prompt(char **environ, t_ret cmd_ret, t_dlist **list, char *copy_buffer)
{
	int		ret;
	size_t	pos;
	char	*line;
	char	buffer[3];

	pos = 0;
	ret = 0;
	line = NULL;
	ft_putstr("$ ");
	while (ret != 1)
	{
		if (read(STDIN_FILENO, buffer, 3) == -1)
			ret = -1;
		if (ret == 0 && buffer[0] != 6 && buffer[0] != 7 && buffer[0] != 8)
			ret = handle_input(buffer, &line, &pos, list);
		else if (ret == 0)
			ret = handle_copy_buffer(buffer, &line, &pos, &copy_buffer);
		if (ret == -1)
		{
			ft_strdel(&copy_buffer);
			free(line);
			return (-1);
		}
	}
	ft_putchar('\n');
	if (line != NULL)
		*list = (*list)->next;
	ft_strdel(&line);
	return (wait_prompt(environ, cmd_ret, list, copy_buffer));
}
