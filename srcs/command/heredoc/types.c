/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   types.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/05 13:56:00 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/05/03 17:04:48 by legrivel    ###    #+. /#+    ###.fr     */
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
	return (input != (int)output ? close(input) : 0);
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
	return (input != (int)output ? close(input) : 0);
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
	return ((int)input != output ? close(output) : 0);
}

int				check_type(char *pointer)
{
	if (*pointer == '>' && *(pointer + 1) != '>' && *(pointer - 1) != '>')
		return (smp_out(pointer - 1, pointer + 1));
	else if (*pointer == '>' && *(pointer + 1) == '>')
		return (dbl_out(pointer - 1, pointer + 2));
	else if (*pointer == '<' && *(pointer + 1) != '<' && *(pointer - 1) != '<')
		return (smp_in(pointer - 1, pointer + 1));
	return (0);
}
