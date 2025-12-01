/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 16:57:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/01 19:19:24 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static t_st		g_textures;
static int		g_textures_ready;

static void	destroy_texture_image(t_mlx *mlx, t_texture *tex)
{
	if (!tex)
		return ;
	if (tex->img && mlx && mlx->mlx_connection)
	{
		mlx_destroy_image(mlx->mlx_connection, tex->img);
		tex->img = NULL;
	}
	tex->addr = NULL;
}

static t_vec2	door_current_span(const t_door *door)
{
	double	angle;

	angle = door->rot_sign * door->progress * HALF_PI;
	return (vec2_rotate(door->span_closed, angle));
}

int	door_ray_intersection(t_game_data *data, t_door *door, double *hit_dist,
		double *u)
{
	t_vec2	origin;
	t_vec2	dir;
	t_vec2	span;
	t_vec2	diff;
	double	denom;
	double	t;
	double	s;

	if (!door || !door->has_geom)
		return (0);
	set_origin_dir(data, &origin, &dir);
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

void	render_3d_view(t_game_data *data, int start_x, int view_width,
		int view_height)
{
	int	x;

	tex_ready(&g_textures_ready, &g_textures, data);
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
		set_current_tex(data, &g_textures);
		draw_walls(data, view_height, x, start_x,
			g_textures.current_tex);
		data->z_buffer[start_x + x] = data->rc.perp_wall_dist;
		x++;
	}
}

void	destroy_textures(t_game_data *data)
{
	if (!g_textures_ready)
		return ;
	destroy_texture_image(&data->mlx, &g_textures.tex_no);
	destroy_texture_image(&data->mlx, &g_textures.tex_so);
	destroy_texture_image(&data->mlx, &g_textures.tex_we);
	destroy_texture_image(&data->mlx, &g_textures.tex_ea);
	destroy_texture_image(&data->mlx, &g_textures.door_tex);
	g_textures.current_tex = NULL;
	g_textures_ready = 0;
	ft_bzero(&g_textures, sizeof(g_textures));
}
