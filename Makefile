NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g

SRCS = \
	src/main.c \
	src/pars/parsing.c \
	src/pars/store_data.c \
	src/pars/valid_map.c \
	src/vectorslib/vec.c \
	src/game/dda_n_helpers.c \
	src/game/door.c \
	src/game/door_geo.c \
	src/game/door_helpers.c \
	src/game/door_helpers1.c \
	src/game/drawing.c \
	src/game/drawing_1.c \
	src/game/drawing_2.c \
	src/game/free_data.c \
	src/game/free_data_1.c \
	src/game/game.c \
	src/game/gun.c \
	src/game/movement.c \
	src/game/movement_1.c \
	src/game/movement_2.c \
	src/game/movement_3.c \
	src/game/raycast.c \
	src/game/raycast_helper.c \
	src/game/raycast_helper_2.c \
	src/game/raycast_helper_3.c \
	src/game/raycast_helper_4.c \
	src/game/raycast_helper_5.c \
	src/game/sprite.c \
	src/game/sprite_helper.c \
	src/game/sprite_helper1.c \
	src/game/sprite_helper2.c \
	src/game/sprite_helper3.c \
	src/game/sprite_helper4.c \
	src/game/drawing_3.c

SRCS_NO_BONUS = \
	src/main_no_bonus.c \
	src/pars/parsing.c \
	src/pars/store_data.c \
	src/pars/valid_map.c \
	src/vectorslib/vec.c \
	src/game_without_bonus/dda_n_helpers.c \
	src/game_without_bonus/drawing.c \
	src/game_without_bonus/drawing_1.c \
	src/game_without_bonus/drawing_2.c \
	src/game_without_bonus/drawing_3.c \
	src/game_without_bonus/free_data.c \
	src/game_without_bonus/free_data_1.c \
	src/game_without_bonus/game.c \
	src/game_without_bonus/movement.c \
	src/game_without_bonus/movement_1.c \
	src/game_without_bonus/movement_2.c \
	src/game_without_bonus/raycast.c \
	src/game_without_bonus/raycast_helper.c \
	src/game_without_bonus/raycast_helper_2.c \
	src/game_without_bonus/raycast_helper_3.c \
	src/game_without_bonus/raycast_helper_4.c \
	src/game_without_bonus/raycast_helper_5.c

SRCS_get =  libft/get_next_line/get_next_line.c libft/get_next_line/get_next_line_utils.c
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