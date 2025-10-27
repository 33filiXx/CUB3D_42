/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 16:57:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/10/25 20:56:10 by rhafidi          ###   ########.fr       */
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

void    get_perp_wall_distance(t_game_data *data)
{
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
    if (data->rc.side == 0)  // vertical wall
        data->rc.wall_x = data->player.pos.y + data->rc.perp_wall_dist * data->rc.ray_dir_y;
    else  // horizontal wall
        data->rc.wall_x = data->player.pos.x + data->rc.perp_wall_dist * data->rc.ray_dir_x;
    // Extract fractional part [0, 1)
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
        exit_error("Texture load failed");
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
    return (tex_x);
}


void    draw_walls(t_game_data *data, int view_height, int view_width)
{
    t_texture tex_no, tex_so, tex_we, tex_ea, current_tex;
    int tex_x;
    
    get_perp_wall_distance(data);
    set_line_height(data, view_height);
    set_drawing_ends(data, view_height);
    set_texture_coordinations(data);
    current_tex = *get_current_texture(data, &tex_no, &tex_so, &tex_we, &tex_ea);
    load_texture(data, &current_tex, get_the_right_texture(data));
    tex_x = flip_text_horizontally(data, &current_tex);
}

void render_3d_view(t_game_data *data, int start_x, int view_width, int view_height)
{
    int x;
    
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
        dda(data);
        draw_walls(data, view_height, view_width);
        x++;
    }
}
    