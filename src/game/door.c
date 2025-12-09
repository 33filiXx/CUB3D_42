/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:18:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/29 20:31:09 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../inc/door.h"

void	door_load_map(t_game_data *data)
{
	int	y;
	int	x;

	check_doors(data);
	data->door_count = 0;
	y = 0;
	while (y < data->map.height)
	{
		x = 0;
		while (x < data->map.width)
		{
			if (data->map.grid[y][x] == 'D')
			{
				if (!append_to_doors(data, y, x))
					return ;
				configure_door_geometry(data, &data->doors[data->door_count
					- 1]);
			}
			x++;
		}
		y++;
	}
}

void	door_toggle(t_game_data *data)
{
	int		map_y;
	int		map_x;
	t_door	*door;

	if (!get_front_cell(data, &map_y, &map_x))
		return ;
	door = find_door(data, map_y, map_x);
	if (!door)
		return ;
	if (door->state == DOOR_CLOSED || door->state == DOOR_CLOSING)
	{
		if (door->state == DOOR_CLOSED)
			door->progress = 0.0;
		door->state = DOOR_OPENING;
	}
	else if (door->state == DOOR_OPEN || door->state == DOOR_OPENING)
	{
		if (door->state == DOOR_OPEN)
			door->progress = 1.0;
		door->state = DOOR_CLOSING;
	}
}

void	update(double dt, t_door *door, bool *changed)
{
	if (door->state == DOOR_OPENING)
	{
		door->progress += door->speed * dt;
		if (door->progress >= 1.0)
		{
			door->progress = 1.0;
			door->state = DOOR_OPEN;
		}
		*changed = true;
	}
	else if (door->state == DOOR_CLOSING)
	{
		door->progress -= door->speed * dt;
		if (door->progress <= 0.0)
		{
			door->progress = 0.0;
			door->state = DOOR_CLOSED;
		}
		*changed = true;
	}
}

void	door_update(t_game_data *data, double dt)
{
	bool	changed;
	size_t	i;
	t_door	*door;

	if (dt < 0)
		dt = 0;
	changed = false;
	i = 0;
	while (i < data->door_count)
	{
		door = &data->doors[i];
		update(dt, door, &changed);
		i++;
	}
	if (changed)
		redraw_map(data);
}

bool	door_is_blocking(t_game_data *data, int map_x, int map_y)
{
	t_door	*door;

	door = find_door(data, map_y, map_x);
	if (!door)
		return (false);
	if (door->state == DOOR_OPEN)
		return (false);
	if (door->state == DOOR_OPENING)
		return (door->progress < 0.95);
	if (door->state == DOOR_CLOSING)
		return (true);
	return (true);
}
