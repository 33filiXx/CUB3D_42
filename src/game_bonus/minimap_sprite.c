/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_sprite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 13:56:38 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	draw_sprite_dot(t_game_data *data, t_minimap *mm, int idx)
{
	int	cx;
	int	cy;
	int	xy[2];

	cx = mm->padding + (int)(data->sprite[idx].position.x * mm->mini_tile);
	cy = mm->padding + (int)(data->sprite[idx].position.y * mm->mini_tile);
	xy[1] = -4;
	while (xy[1] <= 4)
	{
		xy[0] = -4;
		while (xy[0] <= 4)
		{
			if (xy[0] * xy[0] + xy[1] * xy[1] <= 16)
				put_pixel(&data->mlx, cx + xy[0], cy + xy[1], 0xFF0000);
			xy[0]++;
		}
		xy[1]++;
	}
}

void	draw_minimap_sprites(t_game_data *data, t_minimap *minimap)
{
	int	i;

	if (!data->sprite || data->sprite_count <= 0)
		return ;
	i = 0;
	while (i < data->sprite_count)
	{
		draw_sprite_dot(data, minimap, i);
		i++;
	}
}
