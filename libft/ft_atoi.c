/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_atoi.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/26 01:54:19 by legrivel     #+#   ##    ##    #+#       */
/*   Updated: 2017/11/26 01:54:19 by legrivel    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

int		ft_atoi(const char *str)
{
	int		i;
	int		is_positive;
	int		result;

	i = 0;
	while (str[i] == '\n' || str[i] == '\t' || str[i] == '\v'
			|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i += 1;
	result = 0;
	is_positive = str[i] == '-' ? -1 : 1;
	if (str[i] == '+' || str[i] == '-')
		i += 1;
	while (str[i] > 47 && str[i] < 58)
		result = result * 10 + (str[i++] - 48);
	return (result * is_positive);
}
