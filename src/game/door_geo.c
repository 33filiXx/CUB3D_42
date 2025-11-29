/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_geo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:20:16 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/29 20:35:30 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../inc/door.h"

int	evaluate_geometry(t_game_data *data, t_vec2 pivot, t_vec2 span,
		int rot_sign)
{
	t_vec2	span_open;
	t_vec2	sample_mid;
	t_vec2	sample_tip;

	span_open = vec2_rotate(span, rot_sign * HALF_PI);
	sample_mid = vec2_add(pivot, vec2_scale(span_open, 0.5));
	sample_tip = vec2_add(pivot, vec2_scale(span_open, 0.9));
	if (point_hits_wall(data, sample_mid) || point_hits_wall(data, sample_tip))
		return (0);
	return (1);
}

void	init_door_geo(t_game_data *data, t_door *door, t_door_geo *geo)
{
	geo->center = vec2_new(door->map_x + 0.5, door->map_y + 0.5);
	geo->vertical_open = map_is_walkable(data, door->map_y - 1, door->map_x)
		+ map_is_walkable(data, door->map_y + 1, door->map_x);
	geo->horizontal_open = map_is_walkable(data, door->map_y, door->map_x - 1)
		+ map_is_walkable(data, door->map_y, door->map_x + 1);
	if (geo->vertical_open > geo->horizontal_open)
		geo->span_dir = vec2_new(1.0, 0.0);
	else if (geo->horizontal_open > geo->vertical_open)
		geo->span_dir = vec2_new(0.0, 1.0);
	else
		geo->span_dir = vec2_new(1.0, 0.0);
	geo->hinge_signs[0] = -1;
	geo->hinge_signs[1] = 1;
	geo->rot_signs[0] = 1;
	geo->rot_signs[1] = -1;
	geo->best_score = -1;
	geo->best_rot = 1;
	geo->best_pivot = geo->center;
	geo->best_span = geo->span_dir;
}

void	rot_sign_handler(t_game_data *data, t_door_geo *geo, int *j)
{
	int	rot_sign;
	int	score;

	while ((*j) < 2)
	{
		rot_sign = geo->rot_signs[(*j)];
		if (!evaluate_geometry(data, geo->pivot, geo->span, rot_sign))
		{
			(*j)++;
			continue ;
		}
		score = geo->wall_bonus;
		if (score > geo->best_score)
		{
			geo->best_score = score;
			geo->best_pivot = geo->pivot;
			geo->best_span = geo->span;
			geo->best_rot = rot_sign;
		}
		(*j)++;
	}
}

void	hinge_sign_handler(t_game_data *data, t_door *door, t_door_geo *geo)
{
	int	i;
	int	j;

	i = 0;
	while (i < 2)
	{
		geo->hinge_sign = geo->hinge_signs[i];
		set_candidate(geo->center, geo->span_dir, geo->hinge_sign, geo);
		geo->wall_bonus = 0;
		if (geo->span_dir.x != 0.0 && map_is_wall(data, door->map_y, door->map_x
				+ geo->hinge_sign))
			geo->wall_bonus = 1;
		if (geo->span_dir.y != 0.0 && map_is_wall(data, door->map_y
				+ geo->hinge_sign, door->map_x))
			geo->wall_bonus = 1;
		j = 0;
		rot_sign_handler(data, geo, &j);
		i++;
	}
}

void	configure_door_geometry(t_game_data *data, t_door *door)
{
	t_door_geo	geo;

	init_door_geo(data, door, &geo);
	hinge_sign_handler(data, door, &geo);
	if (geo.best_score < 0)
	{
		set_candidate(geo.center, geo.span_dir, -1, &geo);
		geo.best_rot = 1;
	}
	door->pivot = geo.best_pivot;
	door->span_closed = geo.best_span;
	door->rot_sign = geo.best_rot;
	door->has_geom = true;
}
