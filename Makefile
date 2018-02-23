# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2017/12/11 16:25:45 by legrivel     #+#   ##    ##    #+#        #
#    Updated: 2018/02/23 19:01:13 by legrivel    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

NAME = 21sh
SRCS_PATH = ./srcs/
FILES = main.c free_alloc.c prompt/prompt.c prompt/inputs.c \
		prompt/line_manipulation.c prompt/copy.c prompt/movements.c \
		prompt/commands.c prompt/arrows.c prompt/tools.c env.c command/exec.c \
		errors.c command/heredoc.c command/fildes.c signals.c
SRCS = $(addprefix $(SRCS_PATH), $(FILES)) 
OBJS = $(FILES:.c=.o)
INCS_PATH = ./includes/
LIB_PATH = ./libft/
FLAGS = -Wall -Wextra -Werror -Ofast -fsanitize=address
COMPILER = clang

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(LIB_PATH)libft.a $(OBJS)
	$(COMPILER) $(FLAGS) -lcurses -I $(INCS_PATH) $(notdir $(OBJS)) -o $(NAME) libft/libft.a

$(LIB_PATH)libft.a:
	make -C $(LIB_PATH)

./%.o: $(SRCS)
	$(COMPILER) $(FLAGS) -I $(INCS_PATH) -c $(SRCS_PATH)$*.c

clean:
	make -C $(LIB_PATH) clean
	rm -f $(notdir $(OBJS))

fclean: clean
	make -C $(LIB_PATH) fclean
	rm -f $(NAME)

re: fclean all
