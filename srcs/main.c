/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 19:24:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/02 17:42:55 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		main(int argc, char **argv, char **environ)
{
	t_ret	ret;
	t_dlist	*list;

	(void)argv;
	if (argc != 1)
		return (-1);
	if ((environ = ft_tabdup(environ)) == NULL)
		return (-1);
	list = NULL;
	ret.ret = 0;
	ret.stop = 0;
	ret.cmd_ret = 0;
	if (set_canonical() == -1)
	{
		free_everything(&environ, NULL, NULL, NULL);
		return (-1);
	}
	if (wait_prompt(environ, ret, &list) == -1)
	{
		free_everything(&environ, &list, NULL, NULL);
		return (-1);
	}
	free_everything(&environ, &list, NULL, NULL);
	return (0);
}
