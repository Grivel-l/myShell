/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   21sh.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 23:15:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/28 00:45:04 by legrivel    ###    #+. /#+    ###.fr     */
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
# include <fcntl.h>
# include <signal.h>

# define PL 2
# define READ_END 0
# define WRITE_END 1
# define TMP_FILE "/tmp/21sh_tmp"

typedef struct	s_prompt
{
	size_t		pos;
	size_t		extra;
	size_t		quoting;
	char		*line;
	char		buffer[3];
	t_dlist		*commands;
	char		*copy_buffer;
}				t_prompt;

typedef struct	s_command
{
	char		*bin;
	char		**args;
	size_t		exited;
	int			cmd_ret;
	int			fd[2];
	int			fd2[2];
	char		**environ;
}				t_command;

int				set_canonical(char **environ);
void			reset_term(struct termios term);
int				valid_command(t_prompt *prompt);
int				set_options(struct termios term, int options);
int				wait_prompt(t_prompt *prompt, t_command *cmd, struct termios term);

/*
**	Termcaps
*/
int				put_cap(char *cap);
int				goto_end(t_prompt *prompt);
int				clear_all(t_prompt *prompt);
int				write_line(t_prompt *prompt);
int				next_line(char **line, size_t *pos);
size_t			to_line_end_length(t_prompt *prompt);
size_t			to_line_start_length(t_prompt *prompt);

int				isquoting(t_dlist *list);

int				left_arrow(t_prompt *prompt);
int				right_arrow(t_prompt *prompt);
int				rewind_cursor(t_prompt *prompt, size_t length);
int				forward_cursor(t_prompt *prompt, size_t length);

int				handle_ccp(t_prompt *prompt);
int				handle_arrows(t_prompt *prompt);
int				handle_movements(t_prompt *prompt);
int				handle_input(t_prompt *prompt);
int				handle_signals(t_prompt *prompt);

int				remove_char(char **line, size_t *index);
int				insert_char(char **line, char c, size_t *index);
/*
**	Termcaps
*/

/*
**	Command exec
*/
int				treate_command(t_prompt *prompt, t_command *cmd);
int				exec_bin(t_command *cmd, size_t index, t_list *split);

int				split_heredoc(t_command *cmd, t_list *split, t_prompt *prompt, size_t index);

int				check_builtins(t_command *cmd);
int				exec_builtin(t_command *cmd, char *full_cmd, size_t index, size_t is_last);

/*
**	Fildes
*/
int				close_fd(int fd[2]);
int				close_all_fd(int fd[2], int fd2[2]);
int				get_fd(char *str, int default_fd);
int				set_stdout_fd(char **args, char *previous);
int				set_stdin_fd(char **file, char ***args, char *previous);
int				configure_fd(t_command *cmd, size_t index, size_t is_last);
/*
**	Fildes
*/
/*
**	Command exec
*/

void			free_everything(t_command *cmd, t_prompt *prompt);

int				set_env(t_command *cmd);
int				unset_env(t_command *cmd);
char			*get_myenv(char *env, char **environ);
int				delete_env(char ***environ, int index);
int				get_env_index(char **environ, char *arg);

void			print_bin(void);
void			not_found(char *name);
void			not_dir(char *path, char *extra);
void			env_enoent(char *command, char *env);
void			eacces_error(char *name, char *extra);
void			enoent_error(char *path, char *extra);

#endif
