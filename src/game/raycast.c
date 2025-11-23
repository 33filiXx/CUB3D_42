/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 16:57:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/23 21:04:27 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static t_vec2   ray_origin(t_game_data *data)
{
    return (data->player.pos);
}

static t_vec2   ray_direction(t_game_data *data)
{
    return (vec2_new(data->rc.ray_dir_x, data->rc.ray_dir_y));
}

static t_vec2   door_current_span(const t_door *door)
{
    double  angle;

    angle = door->rot_sign * door->progress * HALF_PI;
    return (vec2_rotate(door->span_closed, angle));
}

int  door_ray_intersection(t_game_data *data, t_door *door,
        double *hit_dist, double *u)
{
    t_vec2  origin;
    t_vec2  dir;
    t_vec2  span;
    t_vec2  diff;
    double  denom;
    double  t;
    double  s;

    if (!door || !door->has_geom)
        return (0);
    origin = ray_origin(data);
    dir = ray_direction(data);
    span = door_current_span(door);
    denom = vec2_cross(dir, span);
    if (fabs(denom) < 1e-8)
        return (0);
    diff = vec2_sub(door->pivot, origin);
    t = vec2_cross(diff, span) / denom;
    s = vec2_cross(diff, dir) / denom;
    if (t <= 1e-4 || s < 0.0 || s > 1.0)
        return (0);
    if (hit_dist)
        *hit_dist = t;
    if (u)
        *u = s;
    return (1);
}

void render_3d_view(t_game_data *data, int start_x, int view_width, int view_height)
{
    int x;
    static t_st tex;
    static int textures_ready;

    tex_ready(&textures_ready, &tex, data);
    ensure_z_buffer(data, start_x + view_width);
    x = 0;
    while (x < view_width)
    {
        data->rc.camera_x = 2.0 * x / (double)view_width - 1.0;
        set_ray_dir(data);
        set_player_position(data);
        set_steps(data);
        set_ray_dir_xy(data);
        set_horizontal_line_dist(data);
        init_hit_data(data);
        dda(data);
        set_current_tex(data, &tex);
        draw_walls(data, view_height, view_width, x, start_x,
            tex.current_tex);
        data->z_buffer[start_x + x] = data->rc.perp_wall_dist;
        x++;
    }
}
    