/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:42:38 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 20:43:22 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void		draw_player(int color, t_game_data *data, t_minimap *minimap)
{
	t_mlx *mlx = &data->mlx;
	int ty;
	t_cercle cercle;
	int tx;
	
	set_cercle_data(data, minimap, &cercle);
	ty = cercle.c_y - cercle.r;
	while (ty <= cercle.c_y + cercle.r)
	{
		tx = cercle.c_x - cercle.r;
		while (tx <= cercle.c_x + cercle.r)
		{
			int dx = tx - cercle.c_x;
			int dy = ty - cercle.c_y;
			if ((dx * dx + dy * dy) <= cercle.r * cercle.r)
			{
				if (tx >= minimap->padding && ty >= minimap->padding
					&& tx < minimap->padding + minimap->mini_width
					&& ty < minimap->padding + minimap->mini_height)
					put_pixel(mlx, tx, ty, color);
			}
			tx++;
		}
		ty++;
	}
}

void	set_colors(t_game_data *data, unsigned int *floor_color, unsigned int * ceiling_color)
{
	*floor_color = (data->file_data.floor_color[0] << 16)
		| (data->file_data.floor_color[1] << 8)
		| data->file_data.floor_color[2];
	*ceiling_color = (data->file_data.ceiling_color[0] << 16)
		| (data->file_data.ceiling_color[1] << 8)
		| data->file_data.ceiling_color[2];
}
void	init_mini_draw(t_mini_draw *mini, t_minimap *minimap, t_door *door)
{
	
	mini->span = vec2_rotate(door->span_closed,
		door->rot_sign * door->progress * HALF_PI);
	mini->scale = minimap->mini_tile;
	mini->pivot_x = minimap->padding + door->pivot.x * mini->scale;
	mini->pivot_y = minimap->padding + door->pivot.y * mini->scale;
	mini->span = vec2_scale(mini->span, mini->scale);
	mini->steps = minimap->mini_tile * 2;
	if (mini->steps < 4)
		mini->steps = 4;
	mini->step = 1.0 / (double)mini->steps;
	mini->k = 0;
}
void	drawin_miniDoors_helper(t_mini_draw *mini, t_game_data *data, t_minimap *minimap)
{
	int draw_x;
	int draw_y;

	while (mini->dy <= 1)
	{
		mini->dx = -1;
		while (mini->dx <= 1)
		{
			draw_x = mini->px + mini->dx;
			draw_y = mini->py + mini->dy;
			if (draw_x >= minimap->padding
				&& draw_y >= minimap->padding
				&& draw_x < minimap->padding + minimap->mini_width
				&& draw_y < minimap->padding + minimap->mini_height)
				put_pixel(&data->mlx, draw_x, draw_y, 0x553311);
			mini->dx++;
		}
		mini->dy++;
	}
}

void	drawing_mini_doors(t_mini_draw *mini, t_game_data *data, t_minimap *minimap)
{
	while (mini->k <= mini->steps)
	{

		mini->t = (double)mini->k * mini->step;
		mini->pt = vec2_add(vec2_new(mini->pivot_x, mini->pivot_y), vec2_scale(mini->span, mini->t));
		mini->px = (int)round(mini->pt.x);
		mini->py = (int)round(mini->pt.y);
		mini->dy = -1;
		drawin_miniDoors_helper(mini, data, minimap);
		mini->k++;
	}
}