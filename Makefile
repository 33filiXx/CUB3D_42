NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror


SRCS =  src/main.c src/pars/parsing.c src/pars/store_data.c src/vectorslib/vec.c

OBJS = $(SRCS:.c=.o)
OBJS_NO_BONUS = $(SRCS_NO_BONUS:.c=.o)
OBJS_get = $(SRCS_get:.c=.o)

LIBFT_DIR = ./libft
GNL_DIR = $(LIBFT_DIR)/get_next_line
LIBFT = $(LIBFT_DIR)/libft.a
MLX_DIR = ./minilibx-linux

INC_FLAGS = -I./inc -I$(LIBFT_DIR) -I$(MLX_DIR) -I$(GNL_DIR)
LDFLAGS = -L$(MLX_DIR) -lmlx -lX11 -lXext -lm

CFLAGS += $(INC_FLAGS)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(OBJS_get) $(LIBFT)
	$(CC) $(OBJS) $(CFLAGS)  $(OBJS_get) $(LIBFT) $(LDFLAGS) -o $(NAME)

no_bonus: $(OBJS_NO_BONUS) $(OBJS_get) $(LIBFT)
	$(CC) $(OBJS_NO_BONUS) $(CFLAGS) $(OBJS_get) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_NO_BONUS) $(OBJS_get)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

re_no_bonus: fclean no_bonus

.PHONY: all clean fclean re no_bonus re_no_bonus