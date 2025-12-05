/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:16:08 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/05 18:51:40 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../inc/door.h"

int	map_in_bounds(t_game_data *data, int y, int x)
{
	return (x >= 0 && x < data->map.width && y >= 0 && y < data->map.height);
}

int	map_is_wall(t_game_data *data, int y, int x)
{
	if (!map_in_bounds(data, y, x))
		return (1);
	return (data->map.grid[y][x] == '1');
}

int	map_is_walkable(t_game_data *data, int y, int x)
{
	if (!map_in_bounds(data, y, x))
		return (0);
	if (data->map.grid[y][x] == '1')
		return (0);
	return (1);
}

int	point_hits_wall(t_game_data *data, t_vec2 point)
{
	int	x;
	int	y;

	x = (int)floor(point.x);
	y = (int)floor(point.y);
	if (!map_in_bounds(data, y, x))
		return (1);
	return (data->map.grid[y][x] == '1');
}

void	set_candidate(t_vec2 center, t_vec2 span_dir,
					int hinge_sign, t_door_geo *geo)
{
	t_vec2	pivot;
	t_vec2	span;

	pivot = vec2_add(center, vec2_scale(span_dir, 0.5 * hinge_sign));
	if (hinge_sign < 0)
		span = span_dir;
	else
		span = vec2_scale(span_dir, -1.0);
	geo->pivot = pivot;
	geo->span = span;
}
