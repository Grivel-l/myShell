# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2017/12/11 16:25:45 by legrivel     #+#   ##    ##    #+#        #
#    Updated: 2018/03/16 23:00:31 by legrivel    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

NAME = 21sh
SRCS_PATH = ./srcs/
HEADERS = shell.h
INCS_PATH = ./includes/
LIB_PATH = ./libft/
OBJS_PATH = ./objs/
SUBDIRS = prompt command env command/heredoc command/builtins
SRCS = main.c free_alloc.c prompt/prompt.c prompt/inputs.c errors2.c \
		prompt/line_manipulation.c prompt/copy.c prompt/movements.c \
		prompt/commands.c prompt/arrows.c prompt/tools.c command/exec.c \
		errors.c command/heredoc/heredoc.c command/fildes.c specials.c \
		command/builtins/builtins.c env/env_manipulations.c env/delete_env.c \
		command/syntax.c prompt/completion.c command/heredoc/types.c \
		command/builtins/chdir.c prompt/term.c env/replace.c
OBJS = $(addprefix $(OBJS_PATH), $(SRCS:.c=.o))
FLAGS = -Wall -Wextra -Werror -Ofast -fsanitize=address
COMPILER = clang

.PHONY: all clean fclean re Makefile

all: $(NAME)

$(NAME): $(LIB_PATH)libft.a $(OBJS) $(addprefix $(INCS_PATH), $(HEADERS))
	$(COMPILER) $(FLAGS) -lcurses -I $(INCS_PATH) $(OBJS) -o $(NAME) libft/libft.a

$(LIB_PATH)libft.a:
	make -C $(LIB_PATH)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
	@mkdir -p $(OBJS_PATH) $(addprefix $(OBJS_PATH), $(SUBDIRS))
	$(COMPILER) $(FLAGS) -I $(INCS_PATH) -o $@ -c $^

clean:
	make -C $(LIB_PATH) clean
	rm -rf $(OBJS_PATH)

fclean: clean
	make -C $(LIB_PATH) fclean
	rm -f $(NAME)

re: fclean all
