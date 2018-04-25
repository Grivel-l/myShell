/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/21 02:26:56 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/25 22:09:18 by legrivel    ###    #+. /#+    ###.fr     */
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
	while (*pointer != ' ' && *pointer != '>' &&
			*pointer != '<' && *pointer != '\0')
	{
		if (ft_addchar(&str, *pointer) == -1)
			return (NULL);
		pointer += 1;
	}
	if (ft_addchar(&str, '\0') == -1)
		return (NULL);
	return (str);
}

static int		check_access(char *file, int flags)
{
	int		fd;

	if (access(file, W_OK | R_OK) == -1)
	{
		if (errno == EACCES)
		{
			eacces_error(file, NULL);
			return (-3);
		}
	}
	fd = open(file, O_CREAT | O_RDWR | flags, 0666);
	free(file);
	return (fd);
}

int				get_input_fd(char *str, int flags)
{
	char	c;
	int		fd;
	char	*file;

	if ((file = get_after(str)) == NULL)
		return (-1);
	c = file[0];
	if (c == '&')
	{
		if (ft_strcmp(&(file[1]), "-") == 0)
			return (free_and_return(file, -2));
		fd = 1;
		while (file[fd] >= 48 && file[fd] <= 57 && file[fd] != '\0')
			fd += 1;
		if (file[fd] != '\0')
		{
			generic_error(&(file[1]), "ambiguous redirect");
			free(file);
			return (-3);
		}
		fd = ft_atoi(&(file[1]));
		free(file);
	}
	return (c == '&' ? fd : check_access(file, flags));
}

size_t			get_output_fd(char *str, size_t default_fd)
{
	while (*str >= 48 && *str <= 57)
		str -= 1;
	if (*str == ' ')
		str += 1;
	return (*str < 48 || *str > 57 ? default_fd : ft_atoi(str));
}

int				update_args(char *str, char ***args)
{
	char	c;
	size_t	i;
	char	*pointer;

	i = 0;
	pointer = str;
	if (set_str(&str, &i) == 1)
		return (0);
	ft_freetab(args);
	if (i == 0)
	{
		while (*str != '>' && *str != '<')
			str += 1;
		*str = '\0';
		return (ft_pushstr(args, pointer));
	}
	c = pointer[i + 1];
	pointer[i + 1] = '\0';
	if (ft_strsplit_qh(pointer, ' ', args) == -1)
		return (-1);
	pointer[i + 1] = c;
	return (0);
}
