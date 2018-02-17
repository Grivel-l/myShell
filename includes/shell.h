/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   21sh.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 23:15:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/17 01:16:10 by legrivel    ###    #+. /#+    ###.fr     */
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
# include <errno.h>

# define PL 2

typedef struct	s_ret
{
	int			ret;
	int			stop;
	int			builtin;
	int			cmd_ret;
}				t_ret;

typedef struct	s_prompt
{
	size_t		pos;
	size_t		extra;
	char		*line;
	char		buffer[3];
	t_dlist		*commands;
	char		*copy_buffer;
}				t_prompt;

typedef struct	s_command
{
	char		*bin;
	char		**args;
	char		**environ;
}				t_command;

int				set_canonical(void);
void			reset_term(struct termios term);
int				wait_prompt(char **environ, t_ret cmd_ret, t_prompt *prompt);

int				put_cap(char *cap);
int				clear_all(t_prompt *prompt);
int				write_line(t_prompt *prompt);
size_t			to_line_end_length(t_prompt *prompt);
size_t			to_line_start_length(t_prompt *prompt);

int				isquoting(t_dlist *list);

int				left_arrow(t_prompt *prompt);
int				right_arrow(t_prompt *prompt);
int				rewind_cursor(t_prompt *prompt, size_t length);
int				forward_cursor(t_prompt *prompt, size_t length);

int				handle_ccp(t_prompt *prompt);
int				handle_input(t_prompt *prompt);
int				handle_arrows(t_prompt *prompt);
int				handle_movements(t_prompt *prompt);

int				remove_char(char **line, size_t *index);
int				insert_char(char **line, char c, size_t *index);

void			free_everything(char ***environ, t_prompt *prompt);

char			*get_myenv(char *env, char **environ);

int				treate_command(t_prompt *prompt, t_command *cmd);

void			not_found(char *name);

#endif
