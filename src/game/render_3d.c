/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 16:57:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/14 19:06:24 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define HALF_PI 1.5707963267948966

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

static int  door_ray_intersection(t_game_data *data, t_door *door,
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
        {
            data->rc.hit = 1;
            data->rc.kind = HIT_WALL;
        }
        if (data->map.grid[data->rc.map_y][data->rc.map_x] == 'D')
        {
            t_door *door;
            double  dist;
            double  tex_u;

            door = find_door(data, data->rc.map_y, data->rc.map_x);
            if (!door || door->progress >= 0.99)
                continue;
            if (!door_ray_intersection(data, door, &dist, &tex_u))
                continue;
            data->rc.hit = 1;
            data->rc.kind = HIT_DOOR;
            data->rc.hit_door = door;
            data->rc.door_progress = door->progress;
            data->rc.perp_wall_dist = dist;
            data->rc.wall_x = tex_u;
            data->rc.side = 0;
        }
    }
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


void    set_line_height(t_game_data *data, int view_height)
{
    data->rc.line_height = (int)(view_height / data->rc.perp_wall_dist);
}

void    set_drawing_ends(t_game_data *data, int view_height)
{
    data->rc.draw_start = -data->rc.line_height / 2 + view_height / 2;
    if (data->rc.draw_start < 0)
        data->rc.draw_start = 0;

    data->rc.draw_end = data->rc.line_height / 2 + view_height / 2;
    if (data->rc.draw_end >= view_height)
        data->rc.draw_end = view_height - 1;
}
void    set_texture_coordinations(t_game_data *data)
{
    if (data->rc.kind == HIT_DOOR)
        return ;
    if (data->rc.side == 0)  // vertical wall
        data->rc.wall_x = data->player.pos.y + data->rc.perp_wall_dist * data->rc.ray_dir_y;
    else  // horizontal wall
        data->rc.wall_x = data->player.pos.x + data->rc.perp_wall_dist * data->rc.ray_dir_x;
    // Keep only the fractional part within [0, 1)
    data->rc.wall_x -= floor(data->rc.wall_x);
}

char    *get_the_right_texture(t_game_data *data)
{
    char *texture_path;

    if (data->rc.side == 0)  // vertical wall
    {
        if (data->rc.ray_dir_x > 0)
            texture_path = data->file_data.ea_texture;  // facing EAST
        else
            texture_path = data->file_data.we_texture;  // facing WEST
    }
    else  // horizontal wall
    {
        if (data->rc.ray_dir_y > 0)
            texture_path = data->file_data.so_texture;  // facing SOUTH
        else
            texture_path = data->file_data.no_texture;  // facing NORTH
    }
    return (texture_path);
}

void    load_texture(t_game_data *data, t_texture *tex, char *path)
{
    tex->img = mlx_xpm_file_to_image(tex->mlx_connection,
                                      path, &tex->width, &tex->height);
    if (!tex->img)
        (printf("Texture load failed"), exit(1));
    tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, 
                                   &tex->line_len, &tex->endian);
}

t_texture *get_current_texture(t_game_data *data, t_texture *tex_no, t_texture *tex_so, t_texture *tex_we, t_texture *tex_ea)
{
    if (data->rc.side == 0)  // vertical wall
    {
        if (data->rc.ray_dir_x > 0)
            return tex_ea;  // looking EAST
        else
            return tex_we;  // looking WEST
    }
    else  // horizontal wall
    {
        if (data->rc.ray_dir_y > 0)
            return tex_so;  // looking SOUTH
        else
            return tex_no;  // looking NORTH
    }
}

int    flip_text_horizontally(t_game_data *data, t_texture *current_tex)
{
    int tex_x = (int)(data->rc.wall_x * (double)current_tex->width);
    
    if ((data->rc.side == 0 && data->rc.ray_dir_x > 0) ||
        (data->rc.side == 1 && data->rc.ray_dir_y < 0))
    {
        tex_x = current_tex->width - tex_x - 1;
    }
    if (tex_x < 0)
        tex_x = 0;
    else if (tex_x >= current_tex->width)
        tex_x = current_tex->width - 1;
    return (tex_x);
}

void    draw(t_game_data *data ,t_texture *tex, int view_height, int view_width, int x, int start_x)
{
    double tex_step, tex_pos;
    int y;
    unsigned int color;

    tex_step = (double )tex->height / data->rc.line_height;
    tex_pos = (data->rc.draw_start - view_height / 2.0 + data->rc.line_height / 2.0) * tex_step;
    y = data->rc.draw_start;
    while( y <= data->rc.draw_end)
    {
        tex->tex_y = (int )tex_pos;
        if (tex->tex_y < 0)
            tex->tex_y = 0;
        else if (tex->tex_y >= tex->height)
            tex->tex_y = tex->height - 1;
        tex_pos += tex_step;
        color = *(unsigned int *)(tex->addr + tex->tex_y * tex->line_len + tex->tex_x * (tex->bpp / 8));
        put_pixel(&data->mlx, start_x + x, y, color);
        y++;
    }
}

void    color_floor_and_ceiling(t_game_data *data, int view_hieght, int view_width, int start_x, int x)
{
    int y = 0;
    while (y < data->rc.draw_start)
    {
        put_pixel(&data->mlx, start_x + x, y, data->file_data.cc);
        y++;
    }
    y = data->rc.draw_end;
    while (y < view_hieght)
    {
        put_pixel(&data->mlx, start_x + x, y, data->file_data.fc);
        y++;
    }
}


void    draw_walls(t_game_data *data, int view_height, int view_width,
            int x, int start_x, t_texture *current_tex)
{
    (void)view_width;
    get_perp_wall_distance(data);
    set_line_height(data, view_height);
    set_drawing_ends(data, view_height);
    set_texture_coordinations(data);
    if (data->rc.kind == HIT_DOOR)
    {
        int tex_x;

        tex_x = (int)(data->rc.wall_x * current_tex->width);
        if (tex_x < 0)
            tex_x = 0;
        else if (tex_x >= current_tex->width)
            tex_x = current_tex->width - 1;
        current_tex->tex_x = tex_x;
    }
    else
        current_tex->tex_x = flip_text_horizontally(data, current_tex);
    draw(data, current_tex, view_height, view_width, x, start_x);
    color_floor_and_ceiling(data, view_height, view_width, start_x, x);
}

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

void render_3d_view(t_game_data *data, int start_x, int view_width, int view_height)
{
    int x;
    static t_st tex;
    static int textures_ready;

    tex_ready(&textures_ready, &tex, data);
    x = 0;
    while (x < view_width)
    {
        data->rc.camera_x = 2.0 * x / (double)view_width - 1.0;
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
        data->rc.hit = 0;
        data->rc.kind = HIT_NONE;
        data->rc.hit_door = NULL;
        data->rc.door_progress = 0.0;
        dda(data);
        if (data->rc.kind == HIT_DOOR)
            tex.current_tex = &tex.door_tex;
        else
            tex.current_tex = get_current_texture(data, &tex.tex_no, &tex.tex_so,
                &tex.tex_we, &tex.tex_ea);
        draw_walls(data, view_height, view_width, x, start_x,
            tex.current_tex);
        x++;
    }
}
    