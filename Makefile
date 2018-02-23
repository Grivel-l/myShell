# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2017/12/11 16:25:45 by legrivel     #+#   ##    ##    #+#        #
#    Updated: 2018/02/23 23:09:19 by legrivel    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

NAME = 21sh
SRCS_PATH = ./srcs/
HEADERS = shell.h
INCS_PATH = ./includes/
LIB_PATH = ./libft/
OBJS_PATH = ./objs/
SUBDIRS = prompt command
SRCS = main.c free_alloc.c prompt/prompt.c prompt/inputs.c \
		prompt/line_manipulation.c prompt/copy.c prompt/movements.c \
		prompt/commands.c prompt/arrows.c prompt/tools.c env.c command/exec.c \
		errors.c command/heredoc.c command/fildes.c signals.c
OBJS = $(addprefix $(OBJS_PATH), $(SRCS:.c=.o))
HEADERS = shell.h
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
