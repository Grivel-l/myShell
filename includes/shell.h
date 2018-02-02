/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   21sh.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 23:15:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/02 17:36:32 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <termios.h>
# include <term.h>

typedef struct	s_ret
{
	int				ret;
	int				stop;
	int				builtin;
	int				cmd_ret;
}				t_ret;

int				set_canonical(void);
int				wait_prompt(char **environ, t_ret cmd_ret, t_dlist **list);

int				put_cpblt(char *cpblt);
int				handle_input(char buffer[3]);

void			free_everything(char ***environ, t_dlist **list,
		char ***args, t_list **commands);

#endif
