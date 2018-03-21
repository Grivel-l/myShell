/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   types.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/05 13:56:00 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 02:29:08 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int		check_range(void)
{
	if (errno == EBADF)
	{
		generic_error("file descriptor out of range", "Bad file descriptor");
		return (-2);
	}
	return (-1);
}

int				smp_out(char *before, char *after)
{
	int		input;
	size_t	output;

	if ((input = get_input_fd(after, O_TRUNC)) == -1)
		return (-1);
	output = get_output_fd(before, STDOUT_FILENO);
	if (input == -3)
		return (-2);
	else if (input == -2)
		return (close(output));
	if (dup2(input, output) == -1)
		return (check_range());
	return (close(input));
}

int				dbl_out(char *before, char *after)
{
	int		input;
	size_t	output;

	if ((input = get_input_fd(after, O_APPEND)) == -1)
		return (-1);
	output = get_output_fd(before, STDOUT_FILENO);
	if (dup2(input, output) == -1)
		return (check_range());
	return (close(input));
}

int				smp_in(char *before, char *after)
{
	size_t	input;
	int		output;

	if ((output = get_input_fd(after, 0)) == -1)
		return (-1);
	input = get_output_fd(before, STDIN_FILENO);
	if (output == -3)
		return (-2);
	else if (output == -2)
		return (close(input));
	if (dup2(output, input) == -1)
		return (check_range());
	return (close(output));
}
