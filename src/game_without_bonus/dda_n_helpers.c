/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_n_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:31:32 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/03 22:51:14 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	set_horizontal_line_dist(t_game_data *data)
{
	if (data->rc.step_y > 0)
		data->rc.side_dist_y = fabs((data->rc.map_y + 1 - data->player.pos.y)
				* data->rc.delta_dist_y);
	else
		data->rc.side_dist_y = fabs((data->player.pos.y - data->rc.map_y)
				* data->rc.delta_dist_y);
}

void	set_next_line(t_game_data *data)
{
	if (data->rc.side_dist_x < data->rc.side_dist_y)
	{
		data->rc.side_dist_x += data->rc.delta_dist_x;
		data->rc.map_x += data->rc.step_x;
		data->rc.side = 0;
	}
	else
	{
		data->rc.side_dist_y += data->rc.delta_dist_y;
		data->rc.map_y += data->rc.step_y;
		data->rc.side = 1;
	}
}

int	check_bounds(t_game_data *data)
{
	if (data->rc.map_x < 0 || data->rc.map_x >= data->map.width
		|| data->rc.map_y < 0 || data->rc.map_y >= data->map.height)
		return (1);
	return (0);
}

void	dda(t_game_data *data)
{
	while (!data->rc.hit)
	{
		if (check_bounds(data))
			break ;
		set_next_line(data);
		if (check_bounds(data))
			break ;
		if (data->map.grid[data->rc.map_y][data->rc.map_x] == '1')
			hit_wall(data);
	}
}
