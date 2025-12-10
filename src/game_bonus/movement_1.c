/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:39:13 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 20:52:27 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	rotate_view(t_game_data *data, int direction)
{
	double	angle;
	double	old_dir_x;
	double	old_plane_x;

	angle = direction * data->player.rot_speed;
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

static void	handle_key_press_ext(int keycode, t_game_data *data)
{
	if (keycode == XK_a)
		data->player.strafing_left = 1;
	else if (keycode == XK_d)
		data->player.strafing_right = 1;
	else if (keycode == XK_Left)
		data->player.rotating_left = 1;
	else if (keycode == XK_Right)
		data->player.rotating_right = 1;
	else if (keycode == XK_m)
		data->mouse.mouse_locked = !data->mouse.mouse_locked;
	else if (keycode == XK_e)
		door_toggle(data);
}

int	key_press(int keycode, void *param)
{
	t_game_data	*data;

	data = (t_game_data *)param;
	if (keycode == XK_Escape)
	{
		cleanup_game(data);
		exit(0);
	}
	else if (keycode == XK_w)
		data->player.moving_forward = 1;
	else if (keycode == XK_s)
		data->player.moving_backward = 1;
	else
		handle_key_press_ext(keycode, data);
	return (0);
}

static void	handle_key_release_ext(int keycode, t_game_data *data)
{
	if (keycode == XK_a)
		data->player.strafing_left = 0;
	else if (keycode == XK_d)
		data->player.strafing_right = 0;
	else if (keycode == XK_Left)
		data->player.rotating_left = 0;
	else if (keycode == XK_Right)
		data->player.rotating_right = 0;
}

int	key_release(int keycode, void *param)
{
	t_game_data	*data;

	data = (t_game_data *)param;
	if (keycode == XK_w)
		data->player.moving_forward = 0;
	else if (keycode == XK_s)
		data->player.moving_backward = 0;
	else
		handle_key_release_ext(keycode, data);
	return (0);
}
