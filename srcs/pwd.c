/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   pwd.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/31 15:30:40 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2018/01/31 17:55:01 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int			update_env(char *key, t_list *value, char ***environ)
{
	int		ret;
	char	*tmp;
	int		index;

	ret = 0;
	if ((tmp = ft_strjoin(key, "=")) == NULL)
		return (-1);
	if ((tmp = ft_strrealloc(tmp, value->content)) == NULL)
		ret = -1;
	if (ret == 0 && (index = get_env_index(environ, key)) == -1)
	{
		if (ft_pushstr(environ, tmp) == -1)
			ret = -1;
	}
	else
	{
		if (ret == 0 &&
	(delete_env(environ, index) == -1 || ft_pushstr(environ, tmp) == -1))
			ret = -1;
	}
	free(tmp);
	return (ret);
}

int			update_pwd(char ***environ)
{
	int		ret;
	t_list	*pwd;
	char	*tmp;

	ret = 0;
	if ((pwd = get_env(*environ, "PWD", 0)) == NULL)
		return (-1);
	if (update_env("OLDPWD", pwd, environ) == -1)
		ret = -1;
	ft_lstfree(&pwd);
	tmp = ret == 0 ? getcwd(NULL, 0) : NULL;
	if ((pwd = ft_lstnew(tmp, tmp == NULL ? 0 : ft_strlen(tmp))) == NULL)
	{
		ft_strdel(&tmp);
		if (pwd == NULL)
			return (-1);
		if (pwd->content == NULL)
			ret = -1;
	}
	if (ret == 0 && update_env("PWD", pwd, environ) == -1)
		ret = -1;
	ft_strdel(&tmp);
	ft_lstfree(&pwd);
	return (ret);
}
