/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:41:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/27 20:44:17 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"


void draw_minimap_door(t_game_data *data, t_minimap *minimap, t_door *door)
{
	t_mini_draw mini;

	if (!door || !door->has_geom)
		return ;
	init_mini_draw(&mini, minimap, door);
	drawing_mini_doors(&mini, data, minimap);
}

void	draw_tile(t_game_data *data, int i, int j, int color, t_minimap *minimap)
{
	int tx;
	int ty;


	ty = 0;
	while (ty < minimap->mini_tile)
	{
		tx = 0;
		while (tx < minimap->mini_tile)
		{
			put_pixel(&data->mlx, minimap->padding +j * minimap->mini_tile + tx,
				 minimap->padding + i * minimap->mini_tile + ty, color);
			tx++;
		}
		ty++;
	}
	if (data->map.grid[i][j] == 'D')
	{
		t_door *door;

		door = find_door(data, i, j);
		if (door)
			draw_minimap_door(data, minimap, door);
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
void	set_right_color(t_game_data *data, int i , int j , int *color)
{
	if (data->map.grid[i][j] == '0' || data->map.grid[i][j] == 'N' || data->map.grid[i][j] == 'S' || data->map.grid[i][j] == 'E' || data->map.grid[i][j] == 'W')
		(*color) = data->file_data.fc;
	else if (data->map.grid[i][j] == '1')
		(*color) = data->file_data.cc;
	else
		(*color) = 0;
}

void	draw_env(t_game_data *data)
{
	int	i;
	int	j;
	int	color;
	t_minimap	minimap;

	i = 0;
	j = 0;
	adjust_dimensions(&minimap, data);
	set_colors(data, &data->file_data.fc, &data->file_data.cc);
	while (i < data->map.height)
	{
		j = 0;
		while(j < data->map.width)
		{
			set_right_color(data, i , j , &color);
			draw_tile(data, i, j , color, &minimap);
			j++;
		}
		i++;
	}
	draw_player(0xFF0000, data, &minimap);
}