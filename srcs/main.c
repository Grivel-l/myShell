/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 19:24:07 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/30 22:52:04 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int		main(int argc, char **argv, char **environ)
{
	t_ret	ret;

	(void)argv;
	if (argc != 1)
		return (-1);
	if ((environ = ft_tabdup(environ)) == NULL)
		return (-1);
	ret.stop = 0;
	ret.cmd_ret = 0;
	if (wait_prompt(environ, ret) == -1)
	{
		free_everything(&environ, NULL, NULL, NULL);
		printf("Error\n");
		return (-1);
	}
	free_everything(&environ, NULL, NULL, NULL);
	return (0);
}
