/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   echo.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 01:41:05 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/09 19:06:55 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	print_char(t_quote *quotes, char **str, t_command *cmd)
{
	char	*tmp;

	if (**str == '$' && *((*str) + 1) == '?')
	{
		if ((tmp = ft_itoa(cmd->cmd_ret)) == NULL)
			return (-1);
		ft_putstr(tmp);
		free(tmp);
		*str += 1;
		return (0);
	}
	if ((**str == '<' || **str == '>') && !quotes->doubleq && !quotes->simpleq)
	{
		cmd->cmd_ret = 0;
		ft_putchar('\n');
		return (1);
	}
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

static int	set_and_stop(t_command *cmd)
{
	cmd->cmd_ret = 1;
	return (-1);
}

int			echo_builtin(t_command *cmd, char **full_cmd)
{
	int		ret;
	char	*line;
	t_quote	quotes;

	quotes.simpleq = 0;
	quotes.doubleq = 0;
	if (replace_builtin_tilde(full_cmd, cmd->environ) == -1)
		return (-1);
	line = *full_cmd;
	while (*line && *line != ' ')
		line += 1;
	if (*line != '\0')
		line += 1;
	while (*line == ' ')
		line += 1;
	while (*line)
	{
		if ((ret = print_char(&quotes, &line, cmd)) == -1 || ret == 1)
			return (ret == 0 ? 0 : set_and_stop(cmd));
		increase_pointer(&line);
	}
	ft_putchar('\n');
	cmd->cmd_ret = 0;
	return (0);
}
