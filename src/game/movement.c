/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:38:58 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/17 18:00:09 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	on_mouse_move(int x, int y, void *param)
{
	t_game_data	*data;
	int			center_x;
	int			delta;

	data = (t_game_data *)param;
	(void)y;
	if (data->warping_mouse || !data->mouse.mouse_locked)
		return (0);
	center_x = WIDTH / 2;
	delta = x - center_x;
	if (delta >= -2 && delta <= 2)
		return (0);
	if (delta > 50)
		delta = 50;
	else if (delta < -50)
		delta = -50;
	data->mouse.pending_rotation += (double)delta * 0.003;
	data->warping_mouse = 1;
	mlx_mouse_move(data->mlx.mlx_connection, data->mlx.mlx_win,
		center_x, HEIGHT / 2);
	data->warping_mouse = 0;
	return (0);
}

void	init_mouse(t_game_data *data)
{
	data->mouse.pending_rotation = 0.0;
	data->mouse.has_prev_pos = false;
	data->mouse.mouse_locked = true;
	mlx_mouse_move(data->mlx.mlx_connection, data->mlx.mlx_win,
		WIDTH / 2, HEIGHT / 2);
}

static int	cell_is_blocked(t_game_data *data, int map_x, int map_y)
{
	if (map_x < 0 || map_x >= data->map.width)
		return (1);
	if (map_y < 0 || map_y >= data->map.height)
		return (1);
	if (data->map.grid[map_y][map_x] == '1')
		return (1);
	return (0);
}

int	blocked_at(t_game_data *data, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)floor(x);
	map_y = (int)floor(y);
	return (cell_is_blocked(data, map_x, map_y));
}
