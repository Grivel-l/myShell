/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   inputs.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 03:19:24 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/05 18:50:53 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	handle_backspace(char **line, size_t *pos)
{
	if (*pos > 0)
	{
		if (put_cap("le") == -1)
			return (-1);
		if (put_cap("dc") == -1)
			return (-1);
	}
	return (remove_char(line, pos));
}

static int	end_of_line(char **line, size_t *pos)
{
	size_t	length;
	char	buffer[3];

	buffer[2] = 67;
	length = ft_strlen(*line);
	while (*pos < length)
	{
		if (handle_arrows(buffer, line, pos) == -1)
			return (-1);
	}
	return (0);
}

static int	start_of_line(char **line, size_t *pos)
{
	char	buffer[3];

	buffer[2] = 68;
	while (*pos > 0)
		if (handle_arrows(buffer, line, pos) == -1)
			return (-1);
	return (0);
}

static int	handle_arrows(char buffer[3], char **line, size_t *pos)
{
	int		ret;

	ret = 0;
	if (buffer[2] == 68 && *pos > 0)
	{
		*pos -= 1;
		ret = put_cap("le");
	}
	if (buffer[2] == 67 && *line != NULL && *pos < ft_strlen(*line))
	{
		*pos += 1;
		ret = put_cap("nd");
	}
	if (buffer[2] == 70 && *line != NULL)
		ret = end_of_line(line, pos);
	if (buffer[2] == 72 && *line != NULL)
		ret = start_of_line(line, pos);
	return (ret);
}

static int	my_putc(int i)
{
	ft_putchar(i);
	return (0);
}

int			put_cap(char *cap)
{
	char	*str;

	if ((str = tgetstr(cap, NULL)) == NULL)
		return (-1);
	if (tputs(str, 1, my_putc) == -1)
		return (-1);
	return (0);
}

int		handle_input(char buffer[3], char **line, size_t *pos)
{
	if (buffer[0] == 27)
		return (handle_arrows(buffer, line, pos));
	else if (buffer[0] == 127)
		return (handle_backspace(line, pos));
	else if (buffer[0] >= 32 && buffer[0] <= 126)
	{
		if (insert_char(line, buffer[0], pos) == -1)
			return (-1);
		ft_putchar(buffer[0]);
	}
	return (0);
}
