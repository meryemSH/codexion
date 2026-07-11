NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRCS	= src/main.c \
		  src/init.c \
		  src/cleanup.c \
		  src/log.c \
		  src/parcing.c \
		  src/threads.c \
		  src/threads_utils.c \
		  src/monitor.c \
		  src/dongle.c \
		  src/heap_pop.c \
		  src/heap_push.c \
		  src/routine_utils.c \
		  src/scheduler.c \
		  src/remove_waiter.c \
		  src/dongle_utils.c \

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re