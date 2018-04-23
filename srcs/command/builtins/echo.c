/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   echo.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 01:41:05 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/23 18:21:53 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	print_char(t_quote *quotes, char **str)
{
	ft_checkquotes(quotes, **str);
	if (!((**str == '\'' && !quotes->doubleq) ||
				(**str == '"' && !quotes->simpleq)))
		ft_putchar(**str);
	return (0);
}

static void	increase_pointer(char **full_cmd)
{
	while (*((*full_cmd) + 1) != '\0' && *((*full_cmd) + 1) == ' ')
		*full_cmd += 1;
	if (**full_cmd == ' ')
		ft_putchar(' ');
	*full_cmd += 1;
}

int			echo_builtin(t_command *cmd, char **full_cmd)
{
	char	*line;
	t_quote	quotes;

	quotes.simpleq = 0;
	quotes.doubleq = 0;
	line = *full_cmd;
	while (*line && *line != ' ')
		line += 1;
	if (*line != '\0')
		line += 1;
	while (*line == ' ')
		line += 1;
	while (*line)
	{
		print_char(&quotes, &line);
		increase_pointer(&line);
	}
	ft_putchar('\n');
	cmd->cmd_ret = 0;
	return (0);
}
