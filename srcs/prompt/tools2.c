/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools2.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/20 22:35:44 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/20 22:36:03 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

size_t		to_line_start_length(t_prompt *prompt, size_t col, size_t extra)
{
	size_t	pos;

	pos = prompt->pos;
	if (prompt->line[pos] == '\n')
		pos -= 1;
	while (prompt->line[pos] != '\n' && pos > 0)
		pos -= 1;
	if (pos < prompt->pos && prompt->line[pos] == '\n')
		pos += 1;
	if (extra && pos == 0 && ft_strchr(prompt->line, '\n') != NULL)
		prompt->extra = PL;
	while (prompt->pos - pos > col)
		pos += col;
	return (prompt->pos - pos);
}

size_t		to_line_end_length(t_prompt *prompt)
{
	size_t	pos;

	pos = prompt->pos;
	while (prompt->line[pos] != '\n' && prompt->line[pos] != '\0')
		pos += 1;
	return (pos - prompt->pos);
}
