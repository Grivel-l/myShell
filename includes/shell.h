/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   21sh.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/01 23:15:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/25 22:29:25 by legrivel    ###    #+. /#+    ###.fr     */
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
# ifndef linux
#  include <sys/syslimits.h>
# endif

# define PL 2
# define READ_END 0
# define WRITE_END 1
# define TMP_FILE "/tmp/21sh_tmp"

typedef struct		s_prompt
{
	size_t			pos;
	size_t			extra;
	size_t			quoting;
	char			*line;
	char			buffer[3];
	t_dlist			*commands;
	struct termios	term;
	char			*copy_buffer;
}					t_prompt;

typedef struct		s_command
{
	char			*bin;
	char			**args;
	size_t			exited;
	int				cmd_ret;
	int				fd[2];
	size_t			tmp_fd;
	char			**environ;
}					t_command;

int					set_canonical(char **environ);
int					reset_term(struct termios term);
int					valid_command(t_prompt *prompt);
int					set_options(struct termios term, int options);
int					wait_prompt(t_prompt *prompt, t_command *cmd);

/*
**	Termcaps
*/
int					put_cap(char *cap);
int					goto_end(t_prompt *prompt);
int					clear_all(t_prompt *prompt);
int					write_line(t_prompt *prompt);
int					next_line(char **line, size_t *pos);
size_t				to_line_end_length(t_prompt *prompt);
size_t				to_line_start_length(t_prompt *prompt,
			size_t col, size_t extra);

int					isquoting(t_dlist *list);

int					left_arrow(t_prompt *prompt);
int					right_arrow(t_prompt *prompt);
int					rewind_cursor(t_prompt *prompt, size_t length);
int					forward_cursor(t_prompt *prompt, size_t length);

int					handle_ccp(t_prompt *prompt);
int					handle_return(t_prompt *prompt);
int					handle_arrows(t_prompt *prompt);
int					handle_signals(t_prompt *prompt);
int					handle_movements(t_prompt *prompt);
int					handle_printable(t_prompt *prompt, char c);
int					handle_tab(t_prompt *prompt, char **environ);
int					handle_input(t_prompt *prompt, char **environ);

/*
**	Completion
*/
int					is_dir(char *file);
int					free_printed(t_list **printed, int ret);
void				goto_lstend(t_list **match, t_list **last);
int					check_dir(char *content, t_prompt *prompt);
int					complete_arg(t_prompt *prompt, char *path);
size_t				get_match_nbr(t_list *files, char *content);
int					complete_bin(t_prompt *prompt, char **environ);
int					check_complete(t_prompt *prompt, t_list *files);
int					complete_cmd(t_prompt *prompt, t_list *files,
			char *content);
int					half_complete(t_prompt *prompt, t_list *printe,
			size_t index);
int					print_match(t_list *files, char *content,
			size_t *index, t_list **match);
/*
**	Completion
*/

int					remove_char(char **line, size_t *index);
int					insert_char(char **line, char c, size_t *index);
/*
**	Termcaps
*/

/*
**	Command exec
*/
int					free_quit(t_list **pointer);
int					treate_command(t_prompt *prompt, t_command *cmd);
int					dup_entries(int fd[2], int fd2[2], size_t direction);
int					split_pipe(char *command, t_command *cmd, t_prompt *prompt);

/*
**	Bin
*/
int					check_notdir(char *file);
int					check_filetype(char *bin);
int					get_bin_path(t_command *cmd);
int					check_bin(char *path, char **bin, char **error);
int					exec_bin(t_command *cmd, t_list *split);
int					check_dbl_in(t_command *cmd,
			t_list *split, t_prompt *prompt);
int					print_bin_error(char *bin,
			void fun(char *path, char *extra));
/*
**	Bin
*/

/*
**	Heredocs
*/
char				*get_after(char *pointer);
int					set_str(char **str, size_t *i);
int					check_all_heredocs(char *content);
int					smp_in(char *before, char *after);
int					get_input_fd(char *str, int flags);
int					smp_out(char *before, char *after);
int					dbl_out(char *before, char *after);
int					update_args(char *str, char ***args);
size_t				get_output_fd(char *str, size_t default_fd);
int					check_type(char *pointer);
int					stop_read(char **buffer, char **match, char *before);
int					read_set_stdin(char *after, t_prompt *prompt,
			char **environ, char *before);
/*
**	Heredocs
*/

int					check_syntax(t_prompt *prompt);

int					check_builtins(t_command *cmd);
int					exec_builtin(t_command *cmd,
			char **full_cmd, size_t is_last);

/*
**	Fildes
*/
int					close_fd(int fd[2]);
int					get_fd(char *str, int default_fd);
int					close_all_fd(int fd[2], int fd2[2]);
int					set_stdout_fd(char **args, char *previous);
int					set_stdin_fd(char **file, char ***args, char *previous);
int					configure_fd(t_command *cmd, size_t is_last, char *content);
/*
**	Fildes
*/

/*
**	Builtins
*/
int					cd_builtin(t_command *cmd);
int					echo_builtin(t_command *cmd, char **full_cmd);
/*
**	Builtins
*/

/*
**	Command exec
*/

int					free_and_return(char *str, int return_value);
void				free_everything(t_command *cmd, t_prompt *prompt);

/*
** ENV
*/
int					set_env(t_command *cmd);
int					unset_env(t_command *cmd);
void				print_env(t_command *cmd);
int					add_env(char *env, char ***environ);
char				*get_myenv(char *env, char **environ);
int					delete_env(char ***environ, int index);
int					get_env_index(char **environ, char *arg);
int					replace_env(char **line, char **environ);
/*
** ENV
*/

/*
** Errors
*/
void				print_bin(void);
int					syntax_error(char c);
void				not_found(char *name);
int					denied_error(char **error);
void				not_dir(char *path, char *extra);
void				env_enoent(char *command, char *env);
void				eacces_error(char *name, char *extra);
void				enoent_error(char *path, char *extra);
void				generic_error(char *name, char *message);
/*
** Errors
*/

int					replace_cmdret(char **line, int cmd_ret);
int					replace_tilde(char **line, char **environ);
int					replace_all(char **line, char **environ, int cmd_ret);

#endif
