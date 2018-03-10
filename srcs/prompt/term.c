#include "shell.h"

void			reset_term(struct termios term)
{
	put_cap("ei");
	tcsetattr(0, TCSANOW, &term);
}

int				set_canonical(char **environ)
{
	int				ret;
	char			*name;
	struct termios	term;

	if ((name = get_myenv("TERM", environ)) == NULL)
	{
		ft_putstr_fd("$TERM is missing", 2);
		return (-1);
	}
	ret = tgetent(NULL, name);
	if (ret == 1 && tcgetattr(0, &term) == -1)
		ret = -1;
	if (ret == -1 || ret == 0)
		return (-1);
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	if (put_cap("im") == -1)
		return (-1);
	return (0);
}

int			set_options(struct termios term, int options)
{
	term.c_lflag &= ~(options);
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	return (0);
}
