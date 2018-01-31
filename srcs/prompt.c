/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 22:59:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/30 22:59:15 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int				wait_prompt(char **environ, t_ret cmd_ret)
{
	int		ret;
	char	*line;
	char	**tab;
	t_list	*commands;

	ft_putstr("$ ");
	if ((ret = get_next_line(STDIN_FILENO, &line)) == 1)
	{
		if (line[0] != '\0')
		{
			if (ft_strsplit_qh(line, ';', &tab) == -1)
			{
				ft_strdel(&line);
				return (-1);
			}
			ft_strdel(&line);
			commands = ft_tabtolist(tab);
			ft_freetab(&tab);
			if (commands == NULL ||
		exec_commands(commands, &environ, &cmd_ret) == -1)
				return (-1);
		}
	}
	return (ret == -1 ? -1 : wait_prompt(environ, cmd_ret));
}
