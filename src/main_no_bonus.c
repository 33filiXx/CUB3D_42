/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_no_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:19:29 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/03 22:51:15 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	print_file_data(t_file_data data)
{
	int	i;

	printf("NO Texture: %s\n", data.no_texture);
	printf("SO Texture: %s\n", data.so_texture);
	printf("WE Texture: %s\n", data.we_texture);
	printf("EA Texture: %s\n", data.ea_texture);
	printf("maps_size: %d\n", data.s_element_size);
	printf("player postion row: %d\n", data.row);
	printf("column: %d\n", data.column);
	printf("Floor Color:   [%d, %d, %d]\n", data.floor_color[0],
		data.floor_color[1], data.floor_color[2]);
	printf("Ceiling Color: [%d, %d, %d]\n", data.ceiling_color[0],
		data.ceiling_color[1], data.ceiling_color[2]);
	if (data.map)
	{
		printf("\nMap:\t\n");
		i = 0;
		while (data.map[i])
		{
			printf("%s\n", data.map[i]);
			i++;
		}
	}
	else
		printf("Map: (null)\n");
}

void	free_cmp_data(t_cmp_data *cmp)
{
	int	i;

	if (!cmp)
		return ;
	if (cmp->compass)
	{
		i = 0;
		while (cmp->compass[i])
		{
			free(cmp->compass[i]);
			i++;
		}
		free(cmp->compass);
	}
	free(cmp);
}

void	free_double_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_file_data(t_file_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->map)
	{
		i = 0;
		while (data->map[i])
		{
			free(data->map[i]);
			i++;
		}
		free(data->map);
		data->map = NULL;
	}
	free_texture_paths(data);
	free(data);
}

static unsigned int	pack_color(int color[3])
{
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;

	red = (unsigned int)(color[0] & 0xFF);
	green = (unsigned int)(color[1] & 0xFF);
	blue = (unsigned int)(color[2] & 0xFF);
	return ((red << 16) | (green << 8) | blue);
}

static int	trim_row(char *row)
{
	int	len;

	if (!row)
		return (0);
	len = 0;
	while (row[len] && row[len] != '\n')
	{
		if (row[len] == '+')
			row[len] = '1';
		len++;
	}
	row[len] = '\0';
	return (len);
}

static int	normalize_map_rows(t_file_data *file_data, int *out_width)
{
	int	height;
	int	width;
	int	row_len;

	if (!file_data || !file_data->map)
		return (0);
	height = 0;
	width = 0;
	while (file_data->map[height])
	{
		row_len = trim_row(file_data->map[height]);
		if (row_len > width)
			width = row_len;
		height++;
	}
	if (out_width)
		*out_width = width;
	return (height);
}

static bool	is_player_symbol(char tile)
{
	return (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W');
}

static t_vec2	orientation_dir(char orientation)
{
	if (orientation == 'N')
		return (vec2_new(0.0, -1.0));
	if (orientation == 'S')
		return (vec2_new(0.0, 1.0));
	if (orientation == 'E')
		return (vec2_new(1.0, 0.0));
	return (vec2_new(-1.0, 0.0));
}

static void	init_player(t_game_data *game_data, int row, int col,
		char orientation)
{
	t_vec2	perp;

	game_data->player.pos = vec2_new(col + 0.5, row + 0.5);
	game_data->player.dir = orientation_dir(orientation);
	perp = vec2_perpendicular(game_data->player.dir);
	game_data->player.plane = vec2_scale(perp, 0.66);
	game_data->player.move_speed = 0.02;
	game_data->player.rot_speed = 0.05;
}

int	merge_data(t_game_data *game_data, t_file_data *file_data)
{
	int		map_width;
	int		map_height;
	char	orientation;

	if (!game_data || !file_data)
		return (1);
	map_height = normalize_map_rows(file_data, &map_width);
	if (map_height <= 0 || map_width <= 0)
		return (1);
	if (file_data->row < 0 || file_data->row >= map_height
		|| file_data->column < 0 || file_data->column >= map_width)
		return (1);
	orientation = file_data->map[file_data->row][file_data->column];
	if (!is_player_symbol(orientation))
		return (1);
	file_data->map[file_data->row][file_data->column] = '0';
	game_data->file_data = *file_data;
	game_data->map.grid = game_data->file_data.map;
	game_data->map.width = map_width;
	game_data->map.height = map_height;
	game_data->file_data.map_size = map_height;
	game_data->file_data.fc = pack_color(game_data->file_data.floor_color);
	game_data->file_data.cc = pack_color(game_data->file_data.ceiling_color);
	init_player(game_data, file_data->row, file_data->column, orientation);
	game_data->last_time = get_now_seconds();
	return (0);
}

static int	report_error(char *message)
{
	ft_putendl_fd(message, 2);
	return (1);
}

static int	parse_and_merge(t_game_data *game_data, char **av)
{
	t_file_data	*file_data;

	file_data = ft_calloc(1, sizeof(t_file_data));
	if (!file_data)
		return (report_error("Allocation error"));
	if (punisher(av + 1, file_data))
	{
		free_file_data(file_data);
		return (report_error("Parsing failed"));
	}
	if (merge_data(game_data, file_data))
	{
		free_file_data(file_data);
		return (report_error("Failed to merge parsed data"));
	}
	free(file_data);
	return (0);
}

int	main(int ac, char **av)
{
	t_game_data	game_data;

	if (ac != 2)
		return (report_error("Usage: ./cub3D <map.cub>"));
	ft_bzero(&game_data, sizeof(t_game_data));
	if (parse_and_merge(&game_data, av))
		return (1);
	initiate(&game_data.mlx, &game_data);
	redraw_map(&game_data);
	mlx_hook(game_data.mlx.mlx_win, KeyPress, KeyPressMask, key_press,
		&game_data);
	mlx_hook(game_data.mlx.mlx_win, KeyRelease, KeyReleaseMask, key_release,
		&game_data);
	mlx_loop_hook(game_data.mlx.mlx_connection, game_loop, &game_data);
	mlx_hook(game_data.mlx.mlx_win, DestroyNotify, StructureNotifyMask,
		close_window, &game_data);
	mlx_put_image_to_window(game_data.mlx.mlx_connection, game_data.mlx.mlx_win,
		game_data.mlx.img, 0, 0);
	init_mouse(&game_data);
	mlx_hook(game_data.mlx.mlx_win, MotionNotify, PointerMotionMask,
		on_mouse_move, &game_data);
	mlx_loop(game_data.mlx.mlx_connection);
	cleanup_game(&game_data);
	return (0);
}
