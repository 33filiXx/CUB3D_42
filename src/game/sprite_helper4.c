/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 18:24:17 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/01 18:40:19 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

unsigned int	texel(t_texture *tex, int x, int y)
{
	char	*addr;

	if (!tex || !tex->addr || x < 0 || y < 0 || x >= tex->width
		|| y >= tex->height)
		return (0);
	addr = tex->addr + y * tex->line_len + x * (tex->bpp / 8);
	return (*(unsigned int *)addr);
}

bool	render_sprite_x_incr(t_render_sprite *render_s,
						t_game_data *data, t_sprite *sprite)
{
	if (!data->z_buffer || render_s->x < 0
		|| render_s->x >= data->z_buffer_size
		|| data->z_buffer[render_s->x] <= 0.0
		|| sprite->cam_z >= data->z_buffer[render_s->x])
	{
		render_s->x++;
		return (true);
	}
	return (false);
}
