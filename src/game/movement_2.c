/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:40:21 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 20:52:27 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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

void	strafe_move(t_game_data *data, int direction)
{
	double	step_x;
	double	step_y;

	step_x = direction * data->player.dir.y * data->player.move_speed;
	step_y = direction * -data->player.dir.x * data->player.move_speed;
	apply_move_with_slide(data, step_x, step_y);
}
