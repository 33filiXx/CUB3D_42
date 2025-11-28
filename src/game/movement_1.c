/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:39:13 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 20:40:16 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	rotate_right(t_game_data *data)
{
	t_vec2 old_dir, old_plane;
	
	old_dir.x = data->player.dir.x;
	old_dir.y = data->player.dir.y;
	old_plane.x = data->player.plane.x;
	old_plane.y = data->player.plane.y;
	data->player.dir.x = old_dir.x * cos(data->player.rot_speed) - old_dir.y * sin(data->player.rot_speed);
	data->player.dir.y = old_dir.x * sin(data->player.rot_speed) + old_dir.y * cos(data->player.rot_speed);
	data->player.plane.x = old_plane.x * cos(data->player.rot_speed) - old_plane.y * sin(data->player.rot_speed);
	data->player.plane.y = old_plane.x * sin(data->player.rot_speed) + old_plane.y * cos(data->player.rot_speed);
}

void	rotate_left(t_game_data *data)
{
	t_vec2 old_dir, old_plane;
	
	old_dir.x = data->player.dir.x;
	old_dir.y = data->player.dir.y;
	old_plane.x = data->player.plane.x;
	old_plane.y = data->player.plane.y;
	data->player.dir.x = old_dir.x * cos(-data->player.rot_speed) - old_dir.y * sin(-data->player.rot_speed);
	data->player.dir.y = old_dir.x * sin(-data->player.rot_speed) + old_dir.y * cos(-data->player.rot_speed);
	data->player.plane.x = old_plane.x * cos(-data->player.rot_speed) - old_plane.y * sin(-data->player.rot_speed);
	data->player.plane.y = old_plane.x * sin(-data->player.rot_speed) + old_plane.y * cos(-data->player.rot_speed);
}

int key_press(int keycode, void *param)
{
    t_game_data *data = (t_game_data *)param;
    
    if (keycode == XK_w)
		data->player.moving_forward = 1;
	else if (keycode == XK_s)
		data->player.moving_backward = 1;
	else if (keycode == XK_d)
		data->player.rotating_right = 1;
	else if (keycode == XK_a)
		data->player.rotating_left = 1;
	else if (keycode == XK_e)
		door_toggle(data);
    return (0);
}

int key_release(int keycode, void *param)
{
    t_game_data *data = (t_game_data *)param;
    
    if (keycode == XK_w)
		data->player.moving_forward = 0;
	else if (keycode == XK_s)
		data->player.moving_backward = 0;
	else if (keycode == XK_d)
		data->player.rotating_right = 0;
	else if (keycode == XK_a)
		data->player.rotating_left = 0;
    return (0);
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
	if (data->player.rotating_right)
	{
		rotate_right(data);
		(*moved) = true;
	}
    if (data->player.rotating_left)
	{
		rotate_left(data);
		(*moved) = true;
	}
}