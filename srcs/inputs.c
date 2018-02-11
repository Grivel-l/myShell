/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   inputs.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/02 03:19:24 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/11 20:11:49 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int			write_line(char *line, size_t *pos)
{
	size_t	length;

	if (line != NULL)
	{
		ft_putstr(line);
		length = ft_strlen(line) - *pos;
		*pos = ft_strlen(line);
		return (rewind_cursor(pos, length));
	}
	return (0);
}

int			clear_all(size_t *pos, t_dlist *commands)
{
	size_t	old_pos;

	old_pos = *pos;
	*pos += PL;
	if (rewind_cursor(pos, *pos) == -1)
		return (-1);
	if (put_cap("cd") == -1)
		return (-1);
	if (isquoting(commands))
		ft_putstr("> ");
	else
		ft_putstr("$ ");
	*pos = old_pos;
	return (0);
}

static int	handle_backspace(t_prompt *prompt)
{
	if (prompt->pos == 0)
		return (0);
	if (clear_all(&(prompt->pos), prompt->commands) == -1)
		return (-1);
	if (remove_char(&(prompt->line), &(prompt->pos)) == -1)
		return (-1);
	return(write_line(prompt->line, &(prompt->pos)));
}

static int	clear_line(char **line, size_t *pos)
{
	if (rewind_cursor(pos, *pos) == -1)
		return (-1);
	if (put_cap("ce") == -1)
		return (-1);
	ft_strdel(line);
	return (0);
}

static int	previous_command(t_prompt *prompt)
{
	if (prompt->commands != NULL)
	{
		if (clear_line(&(prompt->line), &(prompt->pos)) == -1)
			return (-1);
		if (prompt->commands->previous != NULL)
			prompt->commands = prompt->commands->previous;
		if ((prompt->line = ft_strdup(prompt->commands->content)) == NULL)
			return (-1);
		prompt->pos = ft_strlen(prompt->line);
		ft_putstr(prompt->line);
	}
	return (0);
}

static int	next_command(t_prompt *prompt)
{
	if (prompt->commands != NULL)
	{
		if (clear_line(&(prompt->line), &(prompt->pos)) == -1)
			return (-1);
		if (prompt->commands->next != NULL)
		{
			prompt->commands = prompt->commands->next;
			if ((prompt->line = ft_strdup(prompt->commands->content)) == NULL)
				return (-1);
			prompt->pos = ft_strlen(prompt->line);
			ft_putstr(prompt->line);
		}
	}
	return (0);
}

int			left_arrow(size_t *pos)
{
	if (*pos > 0)
	{
		*pos -= 1;
		return (put_cap("le"));
	}
	return (0);
}

int			right_arrow(t_prompt *prompt)
{
	int		col;

	if (prompt->line != NULL && prompt->pos < ft_strlen(prompt->line))
	{
		if ((col = tgetnum("co")) == -1)
			return (-1);
		prompt->pos += 1;
		if (prompt->pos + PL != 0 && ((prompt->pos + PL) % col) == 0)
			return (put_cap("do"));
		return (put_cap("nd"));
	}
	return (0);
}

int			handle_arrows(t_prompt *prompt)
{
	char	buffer[3];

	buffer[2] = prompt->buffer[2];
	if (buffer[2] == 68)
		return (left_arrow(&(prompt->pos)));
	else if (buffer[2] == 67)
		return (right_arrow(prompt));
	else if (buffer[2] == 70 && prompt->line != NULL)
		return (forward_cursor(prompt, ft_strlen(prompt->line) - prompt->pos));
	else if (buffer[2] == 72 && prompt->line != NULL)
		return (rewind_cursor(&(prompt->pos), prompt->pos));
	else if (buffer[2] == 65)
		return (previous_command(prompt));
	else if (buffer[2] == 66)
		return (next_command(prompt));
	return (0);
}

static int	handle_printable(t_prompt *prompt, char c)
{
	if (clear_all(&(prompt->pos), prompt->commands) == -1)
		return (-1);
	if (insert_char(&(prompt->line), c, &(prompt->pos)) == -1)
		return (-1);
	return(write_line(prompt->line, &(prompt->pos)));
}

static int	next_line(char **line, size_t *pos)
{
	ft_strdel(line);
	*pos = 0;
	ft_putstr("> ");
	return (0);
}

static int	check_quotes(t_prompt *prompt)
{
	int		ret;
	char	*command;
	t_dlist	*pointer;

	if (prompt->commands == NULL)
	{
		if (!ft_quotesclosed(prompt->line) && handle_printable(prompt, '\n') == -1)
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
	ret = 1;
	if ((command = ft_strjoin(prompt->commands->content, prompt->line)) == NULL)
		ret = -1;
	if (ret == 1 && !ft_quotesclosed(command))
		if (handle_printable(prompt, '\n') == -1)
			ret = -1;
	ft_strdel(&command);
	if (ret == 1 && (prompt->commands->content = ft_strrealloc(prompt->commands->content, prompt->line)) == NULL)
		ret = -1;
	if (ft_quotesclosed(prompt->commands->content))
		ret = 2;
	return (ret);
}

static int	handle_return(t_prompt *prompt)
{
	int		ret;
	t_dlist	*new;

	if (prompt->line == NULL)
		return (1);
	while (prompt->commands != NULL && prompt->commands->next != NULL)
		prompt->commands = prompt->commands->next;
	if ((ret = check_quotes(prompt)) == -1)
		return (-1);
	if (ret == 1)
		return (next_line(&(prompt->line), &(prompt->pos)));
	else if (ret == 2)
		return (1);
	if ((new = ft_dlstnew(prompt->line)) == NULL)
		return (-1);
	if (prompt->commands == NULL)
		prompt->commands = new;
	else
	{
		new->previous = prompt->commands->previous;
		new->previous->next = new;
		free(prompt->commands);
		prompt->commands = new;
	}
	if ((prompt->commands->next = ft_dlstnew("")) == NULL)
		return (-1);
	prompt->commands->next->previous = prompt->commands;
	if (isquoting(prompt->commands))
		return (next_line(&(prompt->line), &(prompt->pos)));
	return (1);
}

static int	my_putc(int i)
{
	ft_putchar(i);
	return (0);
}

int			put_cap(char *cap)
{
	char	*str;

	if ((str = tgetstr(cap, NULL)) == NULL)
		return (-1);
	if (tputs(str, 1, my_putc) == -1)
		return (-1);
	return (0);
}

int		handle_input(t_prompt *prompt)
{
	char	buffer[1];

	buffer[0] = prompt->buffer[0];
	if (buffer[0] == 27)
		return (handle_arrows(prompt));
	else if (buffer[0] == 127)
		return (handle_backspace(prompt));
	else if (buffer[0] == 10)
		return (handle_return(prompt));
	else if (buffer[0] >= 32 && buffer[0] <= 126)
		return (handle_printable(prompt, buffer[0]));
	return (0);
}

void			yo(t_dlist *list)
{
	while (list->previous != NULL)
		list = list->previous;
	while (list->next != NULL)
	{
		printf("Command: %s\n\n", list->content);
		list = list->next;
	}
}

int		handle_input2(t_prompt *prompt)
{
	char	b[1];

	b[0] = prompt->buffer[0];
	if ((b[0] >= 32 && b[0] <= 127) || b[0] == 27 || b[0] == 10)
		return (handle_input(prompt));
	else if (b[0] == 6 || b[0] == 7 || b[0] == 8)
		return (handle_copy_buffer(prompt));
	else if (b[0] == 18 || b[0] == 20 || b[0] == 23 || b[0] == 5)
		return (handle_movements(prompt));
	else
		yo(prompt->commands);
	return (0);
}
