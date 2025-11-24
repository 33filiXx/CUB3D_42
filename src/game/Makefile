NAME		= game_demo
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
INCLUDES= -I../../inc -I../../libft -I../../minilibx-linux
LIBFT_DIR= ../../libft
MLX_DIR = ../../minilibx-linux
LIBFT	= $(LIBFT_DIR)/libft.a
MLX_LIB = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

VEC_SRC= ../../src/vectorslib/vec.c
SRCS	= $(wildcard *.c) $(VEC_SRC)
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_LIB) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
