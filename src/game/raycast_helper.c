/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:25:33 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/27 20:27:51 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"


void    tex_ready(int *textures_ready, t_st *tex, t_game_data *data)
{
    if (!*textures_ready)
    {
        tex->tex_no.mlx_connection = data->mlx.mlx_connection;
        tex->tex_so.mlx_connection = data->mlx.mlx_connection;
        tex->tex_we.mlx_connection = data->mlx.mlx_connection;
        tex->tex_ea.mlx_connection = data->mlx.mlx_connection;
        tex->door_tex.mlx_connection = data->mlx.mlx_connection;
        load_texture(data, &tex->door_tex, "textures/door.xpm");
        load_texture(data, &tex->tex_no, data->file_data.no_texture);
        load_texture(data, &tex->tex_so, data->file_data.so_texture);
        load_texture(data, &tex->tex_we, data->file_data.we_texture);
        load_texture(data, &tex->tex_ea, data->file_data.ea_texture);
        *textures_ready = 1;
    }
}
void    set_ray_dir_xy(t_game_data *data)
{
    if(data->rc.ray_dir_x)
        data->rc.delta_dist_x = fabs(1.0 / data->rc.ray_dir_x);
    else
        data->rc.delta_dist_x = 1e30;
    set_vertical_line_dist(data);
    if(data->rc.ray_dir_y)
        data->rc.delta_dist_y = fabs(1.0 / data->rc.ray_dir_y);
    else
        data->rc.delta_dist_y = 1e30;
}

void    init_hit_data(t_game_data *data)
{
    data->rc.hit = 0;
    data->rc.kind = HIT_NONE;
    data->rc.hit_door = NULL;
    data->rc.door_progress = 0.0;
}

void    set_current_tex(t_game_data *data, t_st *tex)
{
    if (data->rc.kind == HIT_DOOR)
        tex->current_tex = &tex->door_tex;
    else
        tex->current_tex = get_current_texture(data, &tex->tex_no, &tex->tex_so,
            &tex->tex_we, &tex->tex_ea);
}

void    get_perp_wall_distance(t_game_data *data)
{
    double offset;

    if (data->rc.kind == HIT_DOOR)
        return ;
    if (data->rc.side == 0)  // vertical wall
    {
        data->rc.perp_wall_dist = (data->rc.map_x - data->player.pos.x 
                                   + (1 - data->rc.step_x) / 2.0) 
                                    / data->rc.ray_dir_x;
    }   
    else  // horizontal wall
    {
        data->rc.perp_wall_dist = (data->rc.map_y - data->player.pos.y 
                                + (1 - data->rc.step_y) / 2.0) 
                                / data->rc.ray_dir_y;
    }
    //wall adjustement for wall height
    if (data->rc.kind == HIT_DOOR)
    {
        offset = data->rc.perp_wall_dist;
        if (offset < 0.001)
            data->rc.perp_wall_dist = 0.001;
    }
}