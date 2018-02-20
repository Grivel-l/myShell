/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strsplit_qh.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/29 18:58:36 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/02/20 23:31:57 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static void	forward(char **str, char c)
{
	while (**str && **str == c)
		*str += 1;
}

static int	push_tab(char ***tab, char **str, int index)
{
	char	*tmp;
	int		quoted;

	quoted = **str == '\'' || **str == '"' ? 1 : 0;
	if ((tmp = malloc(quoted ? index - 1 : index + 1)) == NULL)
		return (-1);
	ft_strncpy(tmp, quoted ? *str + 1 : *str, quoted ? index - 2 : index);
	tmp[quoted ? index - 2 : index] = '\0';
	if (ft_trim(&tmp) == -1)
		return (-1);
	if (ft_pushstr(tab, tmp) == -1)
	{
		free(tmp);
		return (-1);
	}
	free(tmp);
	*str = *str + index;
	return (0);
}

static int	split_str(char ***tab, char *str, char c)
{
	int		i;
	t_quote	quotes;
	char	*pointer;

	i = 0;
	pointer = str;
	quotes.simpleq = 0;
	quotes.doubleq = 0;
	if (*str != c)
	{
		while (*str)
		{
			if (*str == '"' && !quotes.simpleq)
				quotes.doubleq = !quotes.doubleq;
			else if (*str == '\'' && !quotes.doubleq)
				quotes.simpleq = !quotes.simpleq;
			if (*str == c)
				if (!quotes.doubleq && !quotes.simpleq)
					break ;
			str += 1;
			i += 1;
		}
	}
	if (push_tab(tab, &pointer, i) == -1)
		return (-1);
	if (str == pointer)
		pointer += 1;
	if (*pointer == '\0')
		return ((quotes.doubleq || quotes.simpleq));
	return (split_str(tab, pointer, c));
}

int			ft_strsplit_qh(char *str, char c, char ***tab)
{
	int		ret;

	forward(&str, c);
	if ((*tab = malloc(sizeof(char *))) == NULL)
		return (-1);
	**tab = NULL;
	if (str[0] == '\0')
		return (0);
	if ((ret = split_str(tab, str, c)) == -1)
	{
		ft_freetab(tab);
		return (-1);
	}
	return (ret);
}
