/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 20:52:27 by rhafidi          ###   ########.fr       */
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

static void	handle_rotation(t_game_data *data, bool *moved)
{
	if (data->player.rotating_right)
	{
		rotate_view(data, 1);
		(*moved) = true;
	}
	if (data->player.rotating_left)
	{
		rotate_view(data, -1);
		(*moved) = true;
	}
	if (data->mouse.pending_rotation != 0.0)
	{
		rotate_player(data, data->mouse.pending_rotation);
		data->mouse.pending_rotation = 0.0;
		(*moved) = true;
	}
}

static void	handle_strafing(t_game_data *data, bool *moved)
{
	if (data->player.strafing_left)
	{
		strafe_move(data, 1);
		(*moved) = true;
	}
	if (data->player.strafing_right)
	{
		strafe_move(data, -1);
		(*moved) = true;
	}
}

void	set_moved_flag(t_game_data *data, bool *moved)
{
	if (data->player.moving_forward)
	{
		move_forward(data);
		(*moved) = true;
	}
	if (data->player.moving_backward)
	{
		move_backwards(data);
		(*moved) = true;
	}
	handle_strafing(data, moved);
	handle_rotation(data, moved);
}
