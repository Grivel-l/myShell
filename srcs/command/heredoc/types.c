/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   types.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/05 13:56:00 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/06 03:06:29 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

char			*get_after(char *pointer)
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
	if (file[0] == '&')
	{
		if (ft_strcmp(&(file[1]), "-") == 0)
		{
			free(file);
			return (-2);
		}
		fd = 1;
		while (file[fd] >= 48 && file[fd] <= 57 && file[fd] != '\0')
			fd += 1;
		if (file[fd] != '\0')
		{
			ft_putstr_fd("21sh: ", 2);
			ft_putstr_fd(&(file[1]), 2);
			ft_putstr_fd(": ambiguous redirect\n", 2);
			free(file);
			return (-3);
		}
		fd = ft_atoi(&(file[1]));
		free(file);
		return (fd);
	}
	fd = open(file, O_CREAT | O_RDWR | flags, 0666);
	free(file);
	return (fd);
}

size_t	get_output_fd(char *str, size_t default_fd)
{
	while (*str >= 48 && *str <= 57)
		str -= 1;
	if (*str == ' ')
		str += 1;
	return (*str < 48 || *str > 57 ? default_fd : ft_atoi(str));
}

int		smp_out(char *before, char *after)
{
	int		input;
	size_t	output;

	if ((input = get_input_fd(after, O_TRUNC)) == -1)
		return (-1);
	output = get_output_fd(before, STDOUT_FILENO);
	if (input == -3)
		return (0);
	else if (input == -2)
		return (close(output));
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
	size_t	input;
	int		output;

	if ((output = get_input_fd(after, 0)) == -1)
		return (-1);
	input = get_output_fd(before, STDIN_FILENO);
	if (output == -3)
		return (0);
	else if (output == -2)
		return (close(input));
	if (dup2(output, input) == -1)
		return (-1);
	return (close(output));
}

int		dbl_in(t_prompt *prompt, char **environ, char *before, char *after)
{
	return (read_set_stdin(after, prompt, environ, before));
}
