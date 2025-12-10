/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/09 12:00:00 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	handle_rotation(t_game_data *data, bool *moved)
{
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
	if (data->mouse.pending_rotation != 0.0)
	{
		rotate_player(data, data->mouse.pending_rotation);
		data->mouse.pending_rotation = 0.0;
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
	handle_rotation(data, moved);
}
