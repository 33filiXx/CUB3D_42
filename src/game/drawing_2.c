/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:43:14 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/01 19:08:39 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	put_pixel(t_mlx *mlx, int x, int y, int color)
{
	int	offset;

	offset = y * mlx->line_length + x * (mlx->bits_per_pixel / 8);
	*(unsigned int *)(mlx->addr + offset) = color;
}

void	calculate_scale_and_offset(t_map *map, int *scale, int *offset_x,
		int *offset_y)
{
	int	max_scale_x;
	int	max_scale_y;
	int	actual_width;
	int	actual_height;

	max_scale_x = (map->width * TILE) / map->width;
	max_scale_y = (map->height * TILE) / map->height;
	if (max_scale_x < max_scale_y)
		*scale = max_scale_x;
	else
		*scale = max_scale_y;
	actual_width = map->width * (*scale);
	actual_height = map->height * (*scale);
	*offset_x = ((map->width * TILE) - actual_width) / 2;
	*offset_y = ((map->height * TILE) - actual_height) / 2;
}

void	fill_outer_ppixel(t_game_data *data, int i, int j, int floor_color)
{
	int	tx;
	int	ty;

	ty = i * TILE;
	while (ty < (i + 1) * TILE)
	{
		tx = j * TILE;
		while (tx < (j + 1) * TILE)
		{
			put_pixel(&data->mlx, tx, ty, floor_color);
			tx++;
		}
		ty++;
	}
	// data->map.grid[i][j] = '0';
}

void	set_cercle_data(t_game_data *data, t_minimap *minimap, t_cercle *cercle)
{
	cercle->c_x = minimap->padding + (int)(data->player.pos.x
			* minimap->mini_tile);
	cercle->c_y = minimap->padding + (int)(data->player.pos.y
			* minimap->mini_tile);
	cercle->r = minimap->mini_tile / 2;
	if (cercle->r < 2)
		cercle->r = 2;
}

void	hit_wall(t_game_data *data)
{
	data->rc.hit = 1;
	data->rc.kind = HIT_WALL;
}
