NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror


SRCS =  src/main.c src/pars/parsing.c src/pars/store_data.c src/vectorslib/vec.c

OBJS = $(SRCS:.c=.o)
LIB = ./libft/libft.a

all: $(NAME)

$(LIB):
	$(MAKE) -C libft/


$(NAME): $(OBJS) $(LIB)
	$(CC) $(OBJS) $(CFLAGS) $(LIB) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C libft/ 

fclean: clean
	$(MAKE) fclean -C libft/
	rm -f $(NAME)

re: fclean all 

.PHONY: all clean fclean re

.SECONDARY: $(OBJS)