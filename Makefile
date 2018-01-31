# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: legrivel <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2017/12/11 16:25:45 by legrivel     #+#   ##    ##    #+#        #
#    Updated: 2018/01/31 17:58:17 by legrivel    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

NAME = 21sh
SRCS = main.c prompt.c process.c free_alloc.c env.c errors.c builtins.c \
	   usage.c env_manipulations.c bins.c pwd.c
OBJS = $(SRCS:.c=.o)
SRCS_PATH = ./srcs/
INCS_PATH = ./includes/
LIB_PATH = ./libft/
FLAGS = -Wall -Wextra -Werror -Ofast -fsanitize=address
COMPILER = clang

all: $(NAME)

$(NAME): $(LIB_PATH) $(OBJS)
	$(COMPILER) $(FLAGS) -I $(INCS_PATH) $(OBJS) -o $(NAME) libft/libft.a

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
