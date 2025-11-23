/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:31:32 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/23 19:32:18 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void    set_horizontal_line_dist(t_game_data *data)
{
    if (data->rc.step_y > 0)
    data->rc.side_dist_y = fabs((data->rc.map_y + 1 - data->player.pos.y)
    * data->rc.delta_dist_y);
    else
    data->rc.side_dist_y = fabs((data->player.pos.y - data->rc.map_y)
    * data->rc.delta_dist_y);
}

void    set_next_line(t_game_data *data)
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
int check_bounds(t_game_data *data)
{
    if (data->rc.map_x < 0 || data->rc.map_x >= data->map.width
        || data->rc.map_y < 0 || data->rc.map_y >= data->map.height)
        return (1);
    return (0);
}

void    set_door_data(t_game_data *data, t_door *door, double dist, double tex_u)
{
     data->rc.hit = 1;
    data->rc.kind = HIT_DOOR;
    data->rc.hit_door = door;
    data->rc.door_progress = door->progress;
    data->rc.perp_wall_dist = dist;
    data->rc.wall_x = tex_u;
    data->rc.side = 0;
}

void    dda(t_game_data *data)
{
    t_door *door;
    double  dist;
    double  tex_u;

    while (!data->rc.hit)
    {
        if (check_bounds(data))
            break ;
        set_next_line(data);
        if (check_bounds(data))
            break ;
        if (data->map.grid[data->rc.map_y][data->rc.map_x] == '1')
        {
            data->rc.hit = 1;
            data->rc.kind = HIT_WALL;
        }
        if (data->map.grid[data->rc.map_y][data->rc.map_x] == 'D')
        {
            door = find_door(data, data->rc.map_y, data->rc.map_x);
            if (!door || door->progress >= 0.99)
                continue;
            if (!door_ray_intersection(data, door, &dist, &tex_u))
                continue;
            set_door_data(data, door, dist, tex_u);
        }
    }
}