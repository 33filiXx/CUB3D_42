/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   racast_helper_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:33:28 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/23 19:33:45 by rhafidi          ###   ########.fr       */
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

void    set_line_height(t_game_data *data, int view_height)
{
    data->rc.line_height = (int)(view_height / data->rc.perp_wall_dist);
}