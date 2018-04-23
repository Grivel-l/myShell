/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   checks.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/09 19:33:08 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/23 19:08:43 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int			check_dbl_in(t_command *cmd, t_list *split, t_prompt *prompt)
{
	t_quote		quotes;
	char		*pointer;

	quotes.simpleq = 0;
	quotes.doubleq = 0;
	pointer = split->content;
	while (*pointer)
	{
		ft_checkquotes(&quotes, *pointer);
		if (!quotes.simpleq && !quotes.doubleq &&
		*pointer == '<' && *(pointer + 1) == '<' &&
		read_set_stdin(pointer + 2, prompt, cmd->environ, pointer - 1) == -1)
			return (-1);
		pointer += 1;
	}
	if (update_args(split->content, &(cmd->args)) == -1)
		return (-1);
	return (cmd->bin == NULL ? exec_builtin(cmd,
	(char **)(&(split->content)), split->next == NULL) : exec_bin(cmd, split));
}

int			check_all_heredocs(char *content)
{
	int			ret;
	char		*pointer;
	t_quote		quotes;

	pointer = content;
	quotes.simpleq = 0;
	quotes.doubleq = 0;
	while (*pointer)
	{
		ret = 0;
		ft_checkquotes(&quotes, *pointer);
		if (!quotes.simpleq && !quotes.doubleq &&
				(ret = check_type(pointer)) == -1)
			return (-1);
		if (ret == -2)
			return (0);
		pointer += 1;
	}
	return (0);
}
