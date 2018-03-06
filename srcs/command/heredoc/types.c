/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   types.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/05 13:56:00 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/06 02:38:43 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static size_t	get_output_fd(char *str, size_t default_fd)
{
	while (*str >= 48 && *str <= 57)
		str -= 1;
	if (*str == ' ')
		str += 1;
	return (*str < 48 || *str > 57 ? default_fd : ft_atoi(str));
}

static char		*get_after(char *pointer)
{
	char	*str;

	str = NULL;
	while (*pointer == ' ' && *pointer != '\0')
		pointer += 1;
	while (*pointer != ' ' && *pointer != '>' && *pointer != '<' && *pointer != '\0')
	{
		if (ft_addchar(&str, *pointer) == -1)
			return (NULL);
		pointer += 1;
	}
	if (ft_addchar(&str, '\0') == -1)
		return (NULL);
	return (str);
}

static int		get_input_fd(char *str, int flags)
{
	int		fd;
	char	*file;

	if ((file = get_after(str)) == NULL)
		return (-1);
	fd = open(file, O_CREAT | O_RDWR | flags, 0666);
	free(file);
	return (fd);
}

int		smp_out(char *before, char *after)
{
	int		input;
	size_t	output;

	if ((input = get_input_fd(after, O_TRUNC)) == -1)
		return (-1);
	output = get_output_fd(before, STDOUT_FILENO);
	if (dup2(input, output) == -1)
		return (-1);
	return (close(input));
}

int		dbl_out(char *before, char *after)
{
	int		input;
	size_t	output;

	if ((input = get_input_fd(after, O_APPEND)) == -1)
		return (-1);
	output = get_output_fd(before, STDOUT_FILENO);
	if (dup2(input, output) == -1)
		return (-1);
	return (close(input));
}

int		smp_in(char *before, char *after)
{
	int		input;
	size_t	output;

	if ((output = get_input_fd(after, 0)) == -1)
		return (-1);
	input = get_output_fd(before, STDIN_FILENO);
	if (dup2(output, input) == -1)
		return (-1);
	return (close(output));
}

int		dbl_in(t_prompt *prompt, char **environ, char *before, char *after)
{
	(void)before;
	char	*match;

	if ((match = get_after(after)) == NULL)
		return (-1);
	if (read_set_stdin(match, prompt, environ) == -1)
		return (-1);
	free(match);
	return (0);
}
