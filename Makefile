NAME	= codexion
CC		= cc
FLAGS	= -Wall -Wextra -Werror -pthread

SRCS	= main.c init.c cleanup.c log.c parcing.c threads.c
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re