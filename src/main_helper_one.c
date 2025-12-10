/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 04:44:16 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 17:27:11 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	normalize_map_rows(t_file_data *file_data, int *out_width)
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

bool	is_player_symbol(char tile)
{
	return (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W');
}

t_vec2	orientation_dir(char orientation)
{
	if (orientation == 'N')
		return (vec2_new(0.0, -1.0));
	if (orientation == 'S')
		return (vec2_new(0.0, 1.0));
	if (orientation == 'E')
		return (vec2_new(1.0, 0.0));
	return (vec2_new(-1.0, 0.0));
}

void	init_player(t_game_data *game_data, int row, int col,
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
	char	orientation;

	int (map_width), (map_height);
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
