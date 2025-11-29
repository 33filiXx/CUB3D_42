/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:38:58 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/29 20:52:44 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	on_mouse_move(int x, int y, void *param)
{
	t_game_data	*data;
	double		angle;
	int			delta;

	data = (t_game_data *)param;
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
	int	y;
	int	last_mouse_x;

	if (mlx_mouse_get_pos(data->mlx.mlx_connection, data->mlx.mlx_win,
			&last_mouse_x, &y) == 1)
	{
		data->mouse.prev_delta = last_mouse_x;
		data->mouse.has_prev_pos = true;
	}
	else
		data->mouse.has_prev_pos = false;
}

static int	cell_is_blocked(t_game_data *data, int map_x, int map_y)
{
	if (map_x < 0 || map_x >= data->map.width)
		return (1);
	if (map_y < 0 || map_y >= data->map.height)
		return (1);
	if (data->map.grid[map_y][map_x] == '1')
		return (1);
	if (door_is_blocking(data, map_x, map_y))
		return (1);
	return (0);
}

static int	blocked_at(t_game_data *data, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)floor(x);
	map_y = (int)floor(y);
	return (cell_is_blocked(data, map_x, map_y));
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
