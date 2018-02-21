# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2017/12/11 16:25:45 by legrivel     #+#   ##    ##    #+#        #
#    Updated: 2018/02/21 01:05:59 by legrivel    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

NAME = 21sh
SRCS = main.c free_alloc.c prompt.c inputs.c line_manipulation.c copy.c \
	   movements.c commands.c arrows.c tools.c env.c exec.c errors.c \
	   heredoc.c fildes.c
OBJS = $(SRCS:.c=.o)
SRCS_PATH = ./srcs/
INCS_PATH = ./includes/
LIB_PATH = ./libft/
FLAGS = -Wall -Wextra -Werror -Ofast -fsanitize=address
COMPILER = clang

all: $(NAME)

$(NAME): $(LIB_PATH) $(OBJS)
	$(COMPILER) $(FLAGS) -lcurses -I $(INCS_PATH) $(OBJS) -o $(NAME) libft/libft.a

$(LIB_PATH): $(LIB_PATH)libft.a

$(LIB_PATH)libft.a:
	make -C $(LIB_PATH)

%.o: $(SRCS_PATH)%.c
	$(COMPILER) $(FLAGS) -I $(INCS_PATH) -c $?

clean:
	make -C $(LIB_PATH) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIB_PATH) fclean
	rm -f $(NAME)

re: fclean all
