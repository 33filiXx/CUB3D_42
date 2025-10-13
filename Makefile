NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g


SRCS =  src/main.c src/pars/parsing.c src/pars/store_data.c 
SRCS_get =  libft/get_next_line/get_next_line.c libft/get_next_line/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)
OBJS_get = $(SRCS_get:.c=.o)
LIB = ./libft/libft.a
all: $(NAME)

$(LIB):
	$(MAKE) -C libft/


$(NAME): $(OBJS) $(OBJS_get) $(LIB) 
	$(CC) $(OBJS) $(OBJS_get) $(CFLAGS) $(LIB)  -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_get)
	$(MAKE) clean -C libft/ 

fclean: clean
	$(MAKE) fclean -C libft/
	rm -f $(NAME)

re: fclean all 

.PHONY: all clean fclean re

.SECONDARY: $(OBJS) $(OBJS_get)