/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   inputs.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 03:19:24 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/02 17:42:29 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	my_putc(int i)
{
	ft_putchar(i);
	return (0);
}

int	put_cpblt(char *cpblt)
{
	char	*str;

	if ((str = tgetstr(cpblt, NULL)) == NULL)
		return (-1);
	if (tputs(str, 1, my_putc) == -1)
		return (-1);
	return (0);
}

int		handle_arrows(char buffer[3])
{
	int		ret;

	if (buffer[2] == 68)
		ret = put_cpblt("le");
	if (buffer[2] == 67)
		ret = put_cpblt("nd");
	return (ret);
}

static int	handle_backspace(void)
{
	if (put_cpblt("le") == -1)
		return (-1);
	if (put_cpblt("dc") == -1)
		return (-1);
	return (0);
}

int		handle_input(char buffer[3])
{
	if (buffer[0] == 27)
		return (handle_arrows(buffer));
	else if (buffer[0] == 127)
		return (handle_backspace());
	else
		ft_putchar(buffer[0]);
	return (0);
}
