NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIB = ./libft/libft.a


SRCS =  

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIB):
	$(MAKE) -C libft/


$(NAME): $(OBJS) $(LIB)
	$(CC) $(OBJS) -L./minilibx-linux -lmlx -L/usr/lib -lXext -lX11 -lm -lz $(LIB)  -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -Iminilibx-linux -O3 -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C libft/ 

fclean: clean
	$(MAKE) fclean -C libft/
	rm -f $(NAME)

re: fclean all 

.PHONY: all clean fclean re

.SECONDARY: $(OBJS)