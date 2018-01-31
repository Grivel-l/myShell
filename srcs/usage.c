/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   errors2.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/29 17:45:18 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/31 14:41:53 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

void	print_usage(char *extra)
{
	ft_putstr_fd("usage: ", 2);
	ft_putstr_fd(extra, 2);
	ft_putchar_fd('\n', 2);
}

int		check_args(char **args, char *extra, t_ret *ret)
{
	if (args[1] == NULL || (args[1] != NULL && args[2] == NULL))
	{
		ret->cmd_ret = -1;
		print_usage(extra);
		return (-1);
	}
	return (0);
}
