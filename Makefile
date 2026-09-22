NAME = ft_ls
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I includes

LIBFT = ./libft/libft.a

SRCS = main.c

OBJS = ${SRCS:.c=.o}

all:	${NAME} ${OBJS}

$(NAME): $(OBJS)
		@ ${MAKE} -C ./libft
		@ ${CC} ${CFLAGS} ${OBJS} ${LIBFT} -o ${NAME}

clean:
		${MAKE} clean -C ./libft
		@rm -f ${OBJS}

fclean: clean
		@ ${MAKE} fclean -C ./libft
		@ rm -f ft_ls

re:		fclean all

.PHONY: all clean fclean re