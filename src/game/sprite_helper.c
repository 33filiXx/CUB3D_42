/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:30:28 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/01 18:20:55 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	set_cam_z(t_sprite *sprite, t_vec2 rel)
{
	if (sprite->cam_z > 0.1)
	{
		sprite->dist_cam = sqrt(vec2_dot(rel, rel));
		sprite->visible = true;
	}
	else
	{
		sprite->dist_cam = 0.0;
		sprite->visible = false;
	}
}

void	sprite_camera_transform(t_game_data *data, t_sprite *sprite)
{
	t_vec2	rel;
	double	det;
	double	inv_det;

	rel = vec2_sub(sprite->position, data->player.pos);
	det = data->player.plane.x * data->player.dir.y - data->player.dir.x
		* data->player.plane.y;
	if (fabs(det) < 1e-9)
	{
		sprite->visible = false;
		return ;
	}
	inv_det = 1.0 / det;
	sprite->cam_x = inv_det * (data->player.dir.y * rel.x - data->player.dir.x
			* rel.y);
	sprite->cam_z = inv_det * (-data->player.plane.y * rel.x
			+ data->player.plane.x * rel.y);
	set_cam_z(sprite, rel);
}

void	sprite_update_all(t_game_data *data, double dt)
{
	int	i;

	if (!data->sprite || data->sprite_count <= 0)
		return ;
	i = 0;
	while (i < data->sprite_count)
	{
		sprite_update_one(data, &data->sprite[i], dt);
		sprite_camera_transform(data, &data->sprite[i]);
		i++;
	}
}

void	project_to_screen(t_sprite *sprite, int start_x, int v_w, int v_h)
{
	sprite->draw.screen_x = start_x + (int)((v_w / 2.0) * (1.0 + sprite->cam_x
				* sprite->draw.inv_z));
	sprite->draw.sprite_height = (int)fabs(v_h * sprite->draw.inv_z);
	sprite->draw.sprite_width = sprite->draw.sprite_height;
}

void	clamp_drawing_bounds(t_sprite *sprite, int start_x, int v_w, int v_h)
{
	sprite->draw.draw_start_y = -sprite->draw.sprite_height / 2 + v_h / 2;
	sprite->draw.draw_end_y = sprite->draw.sprite_height / 2 + v_h / 2;
	if (sprite->draw.draw_start_y < 0)
		sprite->draw.draw_start_y = 0;
	if (sprite->draw.draw_end_y >= v_h)
		sprite->draw.draw_end_y = v_h - 1;
	sprite->draw.draw_start_x = -sprite->draw.sprite_width / 2
		+ sprite->draw.screen_x;
	sprite->draw.draw_end_x = sprite->draw.sprite_width / 2
		+ sprite->draw.screen_x;
	if (sprite->draw.draw_start_x < start_x)
		sprite->draw.draw_start_x = start_x;
	if (sprite->draw.draw_end_x >= start_x + v_w)
		sprite->draw.draw_end_x = start_x + v_w - 1;
}
