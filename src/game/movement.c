/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:38:58 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 20:40:42 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int on_mouse_move(int x, int y, void *param)
{
	t_game_data *data = (t_game_data *)param;
	double	angle;
	int		delta;

	(void)y;
	if (data->mouse.has_prev_pos == false)
	{
		data->mouse.prev_delta = x;
		data->mouse.has_prev_pos = true;
		return (0);
	}
	delta = x - (int)data->mouse.prev_delta;
	if (delta == 0)
		return (0);
	if (delta > 200)
		delta = 200;
	else if (delta < -200)
		delta = -200;
	angle = (double)delta * 0.03;
	rotate_player(data, angle);
	redraw_map(data);
	data->mouse.prev_delta = x;
	return (0);
}

void	init_mouse(t_game_data *data)
{
	int y;
	int last_mouse_x;
	
	if( mlx_mouse_get_pos(data->mlx.mlx_connection, data->mlx.mlx_win, &last_mouse_x, &y) == 1)
	{
		data->mouse.prev_delta = last_mouse_x;
		data->mouse.has_prev_pos = true;
	}
	else
		data->mouse.has_prev_pos = false;
}

int	valid_move(t_game_data *data, double new_x, double new_y)
{
	int map_x = (int ) new_x;
	int map_y = (int ) new_y;
	
	if (map_x < 0 || map_x >= data->map.width || map_y < 0 || map_y >= data->map.height)
		return (0);
	if (data->map.grid[map_y][map_x] == '1')
		return (0);
	if (door_is_blocking(data, map_x, map_y))
		return (0);
	return (1);
}

void	move_forward(t_game_data *data)
{
	double new_x;
	double new_y;
	
	new_x = data->player.pos.x + (data->player.dir.x * data->player.move_speed);
	new_y = data->player.pos.y + (data->player.dir.y * data->player.move_speed);
	if (valid_move(data, new_x, new_y))
	{
		data->player.pos.x = new_x;
		data->player.pos.y = new_y;
	}
}
void	move_backwards(t_game_data *data)
{
	double new_x;
	double new_y;
	
	new_x = data->player.pos.x - (data->player.dir.x * data->player.move_speed);
	new_y = data->player.pos.y - (data->player.dir.y * data->player.move_speed);
	if (valid_move(data, new_x, new_y))
	{
		data->player.pos.x = new_x;
		data->player.pos.y = new_y;
	}
}