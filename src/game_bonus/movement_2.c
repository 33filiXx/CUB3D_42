/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:40:21 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/01 19:09:24 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	rotate_player(t_game_data *data, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = data->player.dir.x;
	old_plane_x = data->player.plane.x;
	data->player.dir.x = old_dir_x * cos(angle) - data->player.dir.y
		* sin(angle);
	data->player.dir.y = old_dir_x * sin(angle) + data->player.dir.y
		* cos(angle);
	data->player.plane.x = old_plane_x * cos(angle) - data->player.plane.y
		* sin(angle);
	data->player.plane.y = old_plane_x * sin(angle) + data->player.plane.y
		* cos(angle);
}

int	valid_move(t_game_data *data, double new_x, double new_y)
{
	double	r;

	r = PLAYER_COLLISION_RADIUS;
	if (blocked_at(data, new_x, new_y))
		return (0);
	if (blocked_at(data, new_x + r, new_y))
		return (0);
	if (blocked_at(data, new_x - r, new_y))
		return (0);
	if (blocked_at(data, new_x, new_y + r))
		return (0);
	if (blocked_at(data, new_x, new_y - r))
		return (0);
	if (blocked_at(data, new_x + r, new_y + r))
		return (0);
	if (blocked_at(data, new_x + r, new_y - r))
		return (0);
	if (blocked_at(data, new_x - r, new_y + r))
		return (0);
	if (blocked_at(data, new_x - r, new_y - r))
		return (0);
	return (1);
}

static void	apply_move_with_slide(t_game_data *data, double step_x,
		double step_y)
{
	double	base_x;
	double	base_y;

	base_x = data->player.pos.x;
	base_y = data->player.pos.y;
	if (valid_move(data, base_x + step_x, base_y + step_y))
	{
		data->player.pos.x = base_x + step_x;
		data->player.pos.y = base_y + step_y;
		return ;
	}
	if (step_x != 0.0 && valid_move(data, base_x + step_x, base_y))
		base_x += step_x;
	if (step_y != 0.0 && valid_move(data, base_x, base_y + step_y))
		base_y += step_y;
	data->player.pos.x = base_x;
	data->player.pos.y = base_y;
}

void	move_forward(t_game_data *data)
{
	double	step_x;
	double	step_y;

	step_x = data->player.dir.x * data->player.move_speed;
	step_y = data->player.dir.y * data->player.move_speed;
	apply_move_with_slide(data, step_x, step_y);
}

void	move_backwards(t_game_data *data)
{
	double	step_x;
	double	step_y;

	step_x = -(data->player.dir.x * data->player.move_speed);
	step_y = -(data->player.dir.y * data->player.move_speed);
	apply_move_with_slide(data, step_x, step_y);
}
