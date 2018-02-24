/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   errors.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/28 02:59:08 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/24 01:52:01 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void		env_enoent(char *command, char *env)
{
	print_bin();
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(env, 2);
	ft_putstr_fd(" not set\n", 2);
}

void		eacces_error(char *path, char *extra)
{
	print_bin();
	if (extra != NULL)
		ft_putstr_fd(extra, 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Permission denied\n", 2);
}

void		not_found(char *name)
{
	print_bin();
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void		not_dir(char *path, char *extra)
{
	print_bin();
	if (extra != NULL)
		ft_putstr_fd(extra, 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Not a directory\n", 2);
}

void		enoent_error(char *path, char *extra)
{
	print_bin();
	if (extra != NULL)
		ft_putstr_fd(extra, 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}
