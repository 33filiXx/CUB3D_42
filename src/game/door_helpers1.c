/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:08:34 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/23 19:20:09 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/door.h"
#include "../../inc/cub3d.h"

double	get_now_seconds(void)
{
    struct timeval	tv;

    if (gettimeofday(&tv, NULL) != 0)
        return (0.0);
    return ((double)tv.tv_sec + (double)tv.tv_usec / 1000000.0);
}

bool append_to_doors(t_game_data *data, int map_y, int map_x)
{
    t_door *tmp;
    t_door *door;
    size_t  new_count;

    new_count = data->door_count + 1;
    tmp = realloc(data->doors, sizeof(t_door) * new_count);
    if (!tmp)
        return (false);
    data->doors = tmp;
    door = &data->doors[new_count - 1];
    door->map_x = map_x;
    door->map_y = map_y;
    door->progress = 0.0;
    door->speed = 0.1;
    door->state = DOOR_CLOSED;
    door->pivot = vec2_zero();
    door->span_closed = vec2_new(1.0, 0.0);
    door->rot_sign = 1;
    door->has_geom = false;
    data->door_count = new_count;
    return (true);
}

t_door *find_door(t_game_data *data, int m_y, int m_x)
{
    size_t i = 0;
    while (i < data->door_count)
    {
        if (data->doors[i].map_x == m_x && data->doors[i].map_y == m_y)
            return &data->doors[i];
        i++;
    }
    return NULL;
}

bool get_front_cell(t_game_data *data, int *out_y, int *out_x)
{
    double target_x;
    double target_y;
    const double reach = 1.0;

    target_x = data->player.pos.x + data->player.dir.x * reach;
    target_y = data->player.pos.y + data->player.dir.y * reach;
    int map_x = (int)target_x;
    int map_y = (int)target_y;

    if (map_x < 0 || map_x >= data->map.width
        || map_y < 0 || map_y >= data->map.height)
        return false;
    *out_x = map_x;
    *out_y = map_y;
    return true;
}