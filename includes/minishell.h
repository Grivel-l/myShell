/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   minishell.h                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/27 19:25:27 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/31 17:55:03 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>

typedef struct		s_ret
{
	int				ret;
	int				stop;
	int				cmd_ret;
	int				builtin;
}					t_ret;

void				prepare_bin(char **args, t_list *path_env,
			char **environ, t_ret *ret);

int					wait_prompt(char **environ, t_ret ret);

int					exec_commands(t_list *commands,
			char ***environ, t_ret *ret);

int					check_builtin(t_ret *ret, char **args, char ***environ);

void				free_everything(char ***environ,
		char ***args, t_list **commands);

int					delete_env(char ***environ, int index);
t_list				*get_env(char **environ, char *env_name, int parse);
int					get_env_index(char ***environ, char *arg);
int					update_pwd(char ***environ);
int					update_env(char *key, t_list *value, char ***environ);
int					env_builtin(char ***environ, t_ret *ret);
int					setenv_builtin(char ***environ, char **args, t_ret *ret);
int					unsetenv_builtin(char ***environ,
		char **args, t_ret *ret);

void				not_found(char *name);
void				not_dir(char *path, char *extra);
void				eacces_error(char *path, char *extra);
void				enoent_error(char *path, char *extra);

void				print_usage(char *extra);
int					check_args(char **args, char *extra, t_ret *ret);

#endif
