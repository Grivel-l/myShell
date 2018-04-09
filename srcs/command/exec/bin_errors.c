/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bin_errors.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/09 13:45:46 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/09 14:18:48 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		print_bin_error(char *bin, void fun(char *path, char *extra))
{
	fun(bin, NULL);
	return (1);
}

int		check_filetype(char *bin)
{
	struct stat	infos;

	if (stat(bin, &infos) == -1)
		return (-1);
	if (S_ISDIR(infos.st_mode))
	{
		generic_error(bin, "is a directory");
		return (1);
	}
	return (0);
}

int		check_notdir(char *file)
{
	if (errno == ENOTDIR)
	{
		generic_error(file, "Not a directory");
		return (1);
	}
	return (-1);
}
