/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   echo.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 01:41:05 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 01:42:28 by legrivel    ###    #+. /#+    ###.fr     */
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
	if (**str == '\'' && !quotes->doubleq)
		quotes->simpleq = !quotes->simpleq;
	else if (**str == '"' && !quotes->simpleq)
		quotes->doubleq = !quotes->doubleq;
	else
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

int			echo_builtin(t_command *cmd, char *full_cmd)
{
	int		ret;
	t_quote	quotes;

	quotes.simpleq = 0;
	quotes.doubleq = 0;
	while (*full_cmd && *full_cmd != ' ')
		full_cmd += 1;
	if (*full_cmd != '\0')
		full_cmd += 1;
	while (*full_cmd == ' ')
		full_cmd += 1;
	while (*full_cmd)
	{
		if ((ret = print_char(&quotes, &full_cmd, cmd)) == -1)
		{
			cmd->cmd_ret = 1;
			return (-1);
		}
		if (ret == 1)
			return (0);
		increase_pointer(&full_cmd);
	}
	ft_putchar('\n');
	cmd->cmd_ret = 0;
	return (0);
}
