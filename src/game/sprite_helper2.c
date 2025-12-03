/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:31:32 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/03 17:58:31 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	sprite_load_map(t_game_data *data)
{
	int	y;
	int	x;

	y = 0;
	x = 0;
	// free_previous_sprites(data); // when levels are added we ll need this one
	if (data->sprite)
		free(data->sprite);
	data->sprite = NULL;
	data->sprite_count = 0;
	while (y < data->map.height)
	{
		x = 0;
		while (x < data->map.width)
		{
			if (data->map.grid[y][x] == 'I')
				append_to_sprite(data, x, y);
			x++;
		}
		y++;
	}
}

void	sprite_animate(t_sprite *sprite, double dt)
{
	if (!sprite || sprite->frame_count <= 0)
		return ;
	sprite->frame_time += dt;
	if (sprite->frame_time >= sprite->frame_duration)
	{
		sprite->frame_time -= sprite->frame_duration;
		sprite->frame = (sprite->frame + 1) % sprite->frame_count;
	}
}

int	move_to_target(t_game_data *data, t_sprite *sprite, t_vec2 step)
{
	t_vec2	target;

	target = vec2_add(sprite->position, step);
	if (valid_move(data, target.x, target.y))
	{
		sprite->position = target;
		sprite->map_x = (int)floor(sprite->position.x);
		sprite->map_y = (int)floor(sprite->position.y);
		return (1);
	}
	return (0);
}

void	sprite_try_move(t_game_data *data, t_sprite *sprite, t_vec2 step)
{
	t_vec2	test_x;
	t_vec2	test_y;

	if (move_to_target(data, sprite, step) == 1)
		return ;
	test_x = vec2_add(sprite->position, vec2_new(step.x, 0));
	if (valid_move(data, test_x.x, test_x.y))
	{
		sprite->position = test_x;
		sprite->map_x = (int)floor(sprite->position.x);
		sprite->map_y = (int)floor(sprite->position.y);
		return ;
	}
	test_y = vec2_add(sprite->position, vec2_new(0, step.y));
	if (valid_move(data, test_y.x, test_y.y))
	{
		sprite->position = test_y;
		sprite->map_x = (int)floor(sprite->position.x);
		sprite->map_y = (int)floor(sprite->position.y);
		return ;
	}
}

void	sprite_update_one(t_game_data *data, t_sprite *sprite, double dt)
{
	t_vec2	to_player;
	t_vec2	dir;
	t_vec2	step;
	double	len2;

	sprite_animate(sprite, dt);
	to_player = vec2_sub(data->player.pos, sprite->position);
	len2 = vec2_dot(to_player, to_player); // to_player vec dist is : sqrt(len2)
	if (len2 > 1e-6)
	{
		dir = vec2_scale(to_player, 1.0 / sqrt(len2));
		step = vec2_scale(dir, sprite->speed * dt);
		sprite_try_move(data, sprite, step);
	}
}
