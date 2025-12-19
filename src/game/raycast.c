/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 16:57:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/19 18:10:57 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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

void	render_3d_view(t_game_data *data, int start_x, int view_width,
		int view_height)
{
	int	x;

	tex_ready(&data->textures_ready, &data->textures, data);
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
		set_current_tex(data, &data->textures);
		draw_walls(data, get_infos(start_x, x, view_height),
			data->textures.current_tex);
		x++;
	}
}

void	destroy_textures(t_game_data *data)
{
	destroy_texture_image(&data->mlx, &data->textures.tex_no);
	destroy_texture_image(&data->mlx, &data->textures.tex_so);
	destroy_texture_image(&data->mlx, &data->textures.tex_we);
	destroy_texture_image(&data->mlx, &data->textures.tex_ea);
	data->textures.current_tex = NULL;
	data->textures_ready = 0;
	ft_bzero(&data->textures, sizeof(data->textures));
}
