/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 16:57:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/10/24 18:20:54 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"



void    set_ray_dir(t_game_data *data)
{
    data->rc.ray_dir_x = data->player.dir.x + data->player.plane.x * data->rc.camera_x;
    data->rc.ray_dir_y = data->player.dir.y + data->player.plane.y * data->rc.camera_x;
}

void    set_player_position(t_game_data *data)
{
    data->rc.map_x = data->player.pos.x;
    data->rc.map_y = data->player.pos.y;
}

void    set_steps(t_game_data *data)
{
       
    if (data->rc.ray_dir_x < 0)
        data->rc.step_x = -1;
    else
        data->rc.step_x = 1;
    if (data->rc.ray_dir_y < 0)
        data->rc.step_y = -1;
    else
        data->rc.step_y = 1;
}

void    set_vertical_line_dist(t_game_data *data)
{
    if (data->rc.step_x > 0)
    data->rc.side_dist_x = fabs((data->rc.map_x + 1 - data->player.pos.x)
    * data->rc.delta_dist_x);
    else
    data->rc.side_dist_x = fabs((data->player.pos.x - data->rc.map_x)
    * data->rc.delta_dist_x);
}

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

void    dda(t_game_data *data)
{
    while (!data->rc.hit)
    {
        if (check_bounds(data))
            break ;
        set_next_line(data);
        if (check_bounds(data))
            break ;
        if (data->map.grid[data->rc.map_y][data->rc.map_x] == '1')
            data->rc.hit = 1;
    }
}

void render_3d_view(t_game_data *data, int start_x, int view_width, int view_height)
{
    int x;
    
    x = 0;
    while (x < view_width)
    {
        data->rc.camera_x = 2.0 * x / view_width - 1.0;
        set_ray_dir(data);
        set_player_position(data);
        set_steps(data);
        if(data->rc.ray_dir_x)
            data->rc.delta_dist_x = fabs(1.0 / data->rc.ray_dir_x);
        else
            data->rc.delta_dist_x = 1e30;
        set_vertical_line_dist(data);
        if(data->rc.ray_dir_y)
            data->rc.delta_dist_y = fabs(1.0 / data->rc.ray_dir_y);
        else
            data->rc.delta_dist_y = 1e30;
        set_horizontal_line_dist(data);
        dda(data);
        x++;
    }
}
    