/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   inputs.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 03:19:24 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/21 01:22:36 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int	update_command(t_prompt *prompt)
{
	int		ret;
	char	*command;

	ret = 1;
	if (prompt->line != NULL &&
(command = ft_strjoin(prompt->commands->content, prompt->line)) == NULL)
		ret = -1;
	if (prompt->line == NULL)
		command = prompt->commands->content;
	if (ret == 1 && !ft_quotesclosed(command))
		if (handle_printable(prompt, '\n') == -1)
			ret = -1;
	if (prompt->line[0] != '\n')
		ft_strdel(&command);
	if (ret == 1 && (prompt->commands->content =
		ft_strrealloc(prompt->commands->content, prompt->line)) == NULL)
		ret = -1;
	if (ft_quotesclosed(prompt->commands->content))
		ret = 2;
	return (ret);
}

static int	check_quotes(t_prompt *prompt)
{
	t_dlist	*pointer;

	if (prompt->commands == NULL)
	{
		if (!ft_quotesclosed(prompt->line) &&
				handle_printable(prompt, '\n') == -1)
			return (-1);
		return (0);
	}
	pointer = prompt->commands;
	if (prompt->commands->previous != NULL)
		prompt->commands = prompt->commands->previous;
	if (!isquoting(prompt->commands))
	{
		if (!ft_quotesclosed(prompt->line))
			if (handle_printable(prompt, '\n') == -1)
				return (-1);
		prompt->commands = pointer;
		return (0);
	}
	return (update_command(prompt));
}

static int	create_next(t_prompt *prompt)
{
	t_dlist	*new;

	if ((new = ft_dlstnew(prompt->line)) == NULL)
		return (-1);
	if (prompt->commands == NULL)
		prompt->commands = new;
	else
	{
		new->previous = prompt->commands->previous;
		new->previous->next = new;
		free(prompt->commands->content);
		free(prompt->commands);
		prompt->commands = new;
	}
	if ((prompt->commands->next = ft_dlstnew("")) == NULL)
		return (-1);
	return (0);
}

int			handle_return(t_prompt *prompt)
{
	int		ret;

	if (!isquoting(prompt->commands) && prompt->line == NULL)
		return (1);
	while (prompt->commands != NULL && prompt->commands->next != NULL)
		prompt->commands = prompt->commands->next;
	if ((ret = check_quotes(prompt)) == -1)
		return (-1);
	if (ret == 1)
		return (next_line(&(prompt->line), &(prompt->pos)));
	else if (ret == 2)
		return (1);
	if (prompt->commands != NULL && prompt->commands->previous != NULL &&
		ft_strcmp(prompt->commands->previous->content, prompt->line) == 0)
	{
		prompt->commands = prompt->commands->previous;
		return (1);
	}
	if (create_next(prompt) == -1)
		return (-1);
	prompt->commands->next->previous = prompt->commands;
	if (isquoting(prompt->commands))
		return (next_line(&(prompt->line), &(prompt->pos)));
	return (1);
}

int			next_line(char **line, size_t *pos)
{
	ft_strdel(line);
	*pos = 0;
	ft_putstr("> ");
	return (0);
}
