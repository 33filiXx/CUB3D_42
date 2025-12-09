/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:42:38 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/03 22:51:14 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_player(int color, t_game_data *data, t_minimap *minimap)
{
	int			ty;
	t_cercle	cercle;
	int			tx;
	int			dx;
	int			dy;

	ty = get_ty(&cercle, data, minimap);
	while (ty <= cercle.c_y + cercle.r)
	{
		tx = cercle.c_x - cercle.r;
		while (tx <= cercle.c_x + cercle.r)
		{
			dx = tx - cercle.c_x;
			dy = ty - cercle.c_y;
			if ((dx * dx + dy * dy) <= cercle.r * cercle.r)
			{
				if (tx >= minimap->padding && ty >= minimap->padding
					&& tx < minimap->padding + minimap->mini_width
					&& ty < minimap->padding + minimap->mini_height)
					put_pixel(&data->mlx, tx, ty, color);
			}
			tx++;
		}
		ty++;
	}
}

void	set_colors(t_game_data *data, unsigned int *floor_color,
		unsigned int *ceiling_color)
{
	*floor_color = (data->file_data.floor_color[0] << 16)
		| (data->file_data.floor_color[1] << 8)
		| data->file_data.floor_color[2];
	*ceiling_color = (data->file_data.ceiling_color[0] << 16)
		| (data->file_data.ceiling_color[1] << 8)
		| data->file_data.ceiling_color[2];
}
