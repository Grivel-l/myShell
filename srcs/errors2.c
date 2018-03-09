/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   errors2.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/24 01:23:48 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/28 02:22:39 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void		print_bin(void)
{
	ft_putstr_fd("21sh: ", 2);
}

int			syntax_error(t_prompt *prompt, char c)
{
	prompt->commands = prompt->commands->next;
	print_bin();
	ft_putstr_fd("syntax error", 2);
	if (c != 0)
	{
		ft_putstr_fd(" near unexpected token `", 2);
		ft_putchar_fd(c, 2);
		ft_putchar_fd('\'', 2);
	}
	ft_putchar_fd('\n', 2);
	return (1);
}

void		generic_error(char *name, char *message)
{
	print_bin();
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
}
