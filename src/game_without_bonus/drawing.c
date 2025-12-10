/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:41:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/03 22:51:14 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_tile(t_game_data *data, int *int_holder, t_minimap *minimap)
{
	int		tx;
	int		ty;

	ty = 0;
	while (ty < minimap->mini_tile)
	{
		tx = 0;
		while (tx < minimap->mini_tile)
		{
			put_pixel(&data->mlx, minimap->padding + int_holder[1]
				* minimap->mini_tile
				+ tx, minimap->padding + int_holder[0]
				* minimap->mini_tile + ty, int_holder[2]);
			tx++;
		}
		ty++;
	}
}

void	adjust_dimensions(t_minimap *minimap, t_game_data *data)
{
	double	usable_w;
	double	usable_h;
	double	per_cell_w;
	double	per_cell_h;

	minimap->mini_width = (int)(WIDTH * 0.3);
	minimap->mini_height = (int)(HEIGHT * 0.4);
	minimap->padding = 16;
	usable_w = minimap->mini_width - 2 * minimap->padding;
	usable_h = minimap->mini_height - 2 * minimap->padding;
	if (usable_w < 0)
		usable_w = 0;
	if (usable_h < 0)
		usable_h = 0;
	if (data->map.width > 0)
		per_cell_w = usable_w / data->map.width;
	else
		per_cell_w = usable_w;
	if (data->map.height > 0)
		per_cell_h = usable_h / data->map.height;
	else
		per_cell_h = usable_h;
	minimap->mini_tile = (int)fmin(per_cell_w, per_cell_h);
	if (minimap->mini_tile < 1)
		minimap->mini_tile = 1;
}

void	set_right_color(t_game_data *data, int i, int j, int *color)
{
	if (data->map.grid[i][j] == '0' || data->map.grid[i][j] == 'N'
		|| data->map.grid[i][j] == 'S' || data->map.grid[i][j] == 'E'
		|| data->map.grid[i][j] == 'W')
		(*color) = MINI_FLOOR_COLOR;
	else if (data->map.grid[i][j] == '1')
		(*color) = MINI_WALL_COLOR;
	else
		(*color) = 0;
}

static void	draw_minimap_background(t_game_data *data, t_minimap *minimap)
{
	int	x;
	int	y;
	int	max_x;
	int	max_y;

	max_x = minimap->padding * 2 + data->map.width * minimap->mini_tile;
	max_y = minimap->padding * 2 + data->map.height * minimap->mini_tile;
	y = 0;
	while (y < max_y)
	{
		x = 0;
		while (x < max_x)
		{
			put_pixel(&data->mlx, x, y, 0x111111);
			x++;
		}
		y++;
	}
}

void	draw_env(t_game_data *data)
{
	int			i;
	int			j;
	int			*int_holder;
	int			color;
	t_minimap	minimap;

	i = 0;
	j = 0;
	adjust_dimensions(&minimap, data);
	set_colors(data, &data->file_data.fc, &data->file_data.cc);
	draw_minimap_background(data, &minimap);
	while (i < data->map.height)
	{
		j = 0;
		while (j < data->map.width)
		{
			set_right_color(data, i, j, &color);
			int_holder = set_ints(i, j, color);
			draw_tile(data, int_holder, &minimap);
			free(int_holder);
			j++;
		}
		i++;
	}
	draw_player(MINI_PLAYER_COLOR, data, &minimap);
}
