/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 19:24:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 19:33:25 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		main(int argc, char **argv, char **environ)
{
	t_ret		ret;
	t_prompt	prompt;

	(void)argv;
	if (argc != 1)
		return (-1);
	if ((environ = ft_tabdup(environ)) == NULL)
		return (-1);
	ret.ret = 0;
	ret.stop = 0;
	ret.cmd_ret = 0;
	prompt.commands = NULL;
	prompt.copy_buffer = NULL;
	if (set_canonical() == -1)
	{
		free_everything(&environ, NULL, NULL, NULL);
		return (-1);
	}
	if (wait_prompt(environ, ret, &prompt) == -1)
	{
		// free_everything(&environ, &list, NULL, NULL);
		return (-1);
	}
	// free_everything(&environ, &list, NULL, NULL);
	return (0);
}
