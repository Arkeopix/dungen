NAME	=	dungen
CC		=	gcc
SRC		=	dungen.c
OBJ		=	${SRC:.c=.o}
CFLAGS	=	-ansi		\
			-pedantic	\
			-Wall		\
			-Wextra		\
			-std=c11

${NAME}:	${OBJ}
			${CC} ${OBJ} -o ${NAME}
all:		${NAME}

clean:
			rm -vf ${OBJ}

fclean:		clean
			rm -vf ${NAME}

re:			fclean all
