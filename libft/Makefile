# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: legrivel <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/09 18:32:02 by legrivel          #+#    #+#              #
#    Updated: 2018/02/26 17:24:03 by legrivel    ###    #+. /#+    ###.fr      #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRCS_PATH = ./
INCS_PATH = ./
OBJS = ft_isalpha.o ft_memccpy.o ft_memset.o ft_putnbr_fd.o ft_strcmp.o  \
	   ft_strjoin.o ft_strnequ.o ft_strsub.o ft_isascii.o ft_memchr.o  \
	   ft_putchar.o ft_putstr.o  ft_strcpy.o ft_strlcat.o ft_strnew.o  \
	   ft_strtrim.o ft_isdigit.o ft_memcmp.o ft_putchar_fd.o ft_putstr_fd.o  \
	   ft_strdel.o ft_strlen.o ft_strnstr.o ft_tolower.o ft_atoi.o \
	   ft_isprint.o ft_memcpy.o ft_putendl.o ft_strcat.o ft_strdup.o  \
	   ft_strncat.o ft_strrchr.o ft_toupper.o ft_bzero.o ft_itoa.o ft_memdel.o \
	   ft_putendl_fd.o ft_strchr.o ft_strequ.o ft_strncmp.o ft_strsplit.o \
	   ft_isalnum.o ft_memalloc.o ft_memmove.o ft_putnbr.o ft_strclr.o \
	   ft_strncpy.o ft_strstr.o ft_striter.o ft_striteri.o ft_strmap.o \
	   ft_strmapi.o ft_lstnew.o ft_lstdelone.o ft_lstdel.o ft_lstadd.o \
	   ft_lstiter.o ft_lstmap.o ft_swap_int.o ft_print_tab.o \
	   ft_print_int_tab.o ft_isspace.o ft_isupper.o ft_strrealloc.o \
	   ft_tablen.o ft_puttab.o ft_pushstr.o ft_readdir.o ft_lstappend.o \
	   ft_strtolower.o ft_strcmp_ci.o ft_putofft.o ft_putlonglong.o \
	   get_next_line.o ft_tabdup.o ft_freetab.o ft_strsplit_qh.o ft_putlst.o \
	   ft_tabtolist.o ft_lstfree.o ft_strchr_qh.o ft_dlstnew.o \
	   ft_dlstnappend.o ft_dlstpappend.o ft_putdlst.o ft_dlstfree.o \
	   ft_quotesclosed.o ft_trim.o ft_strsplit_qhv.o

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: $(SRCS_PATH)%.c
	gcc $(FLAGS) -I $(INCS_PATH) -c $(SRCS_PATH)$*.c

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
