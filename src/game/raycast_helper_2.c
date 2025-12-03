/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_helper_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:28:12 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/03 19:39:20 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	flip_text_horizontally(t_game_data *data, t_texture *current_tex)
{
	int	tex_x;

	tex_x = (int)(data->rc.wall_x * (double)current_tex->width);
	if ((data->rc.side == 0 && data->rc.ray_dir_x > 0) || (data->rc.side == 1
			&& data->rc.ray_dir_y < 0))
	{
		tex_x = current_tex->width - tex_x - 1;
	}
	if (tex_x < 0)
		tex_x = 0;
	else if (tex_x >= current_tex->width)
		tex_x = current_tex->width - 1;
	return (tex_x);
}

void	draw(t_game_data *data, t_texture *tex, t_infos infos)
{
	int				y;
	unsigned int	color;
	double			tex_step;
	double			tex_pos;

	tex_step = (double)tex->height / data->rc.line_height;
	tex_pos = (data->rc.draw_start - infos.view_height
			/ 2.0 + data->rc.line_height / 2.0) * tex_step;
	y = data->rc.draw_start;
	while (y <= data->rc.draw_end)
	{
		tex->tex_y = (int)tex_pos;
		if (tex->tex_y < 0)
			tex->tex_y = 0;
		else if (tex->tex_y >= tex->height)
			tex->tex_y = tex->height - 1;
		tex_pos += tex_step;
		color = *(unsigned int *)(tex->addr + tex->tex_y * tex->line_len
				+ tex->tex_x * (tex->bpp / 8));
		put_pixel(&data->mlx, infos.start_x + infos.x, y, color);
		y++;
	}
}

void	color_floor_and_ceiling(t_game_data *data, int view_hieght,
						int start_x, int x)
{
	int	y;

	y = 0;
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

bool	ensure_z_buffer(t_game_data *data, int width)
{
	if (data->z_buffer && data->z_buffer_size == width)
		return (true);
	free(data->z_buffer);
	data->z_buffer = malloc(sizeof(double) * width);
	data->z_buffer_size = width;
	return (data->z_buffer != NULL);
}

void	draw_walls(t_game_data *data, t_infos infos, t_texture *current_tex)
{
	int	tex_x;

	get_perp_wall_distance(data);
	set_line_height(data, infos.view_height);
	set_drawing_ends(data, infos.view_height);
	set_texture_coordinations(data);
	if (data->rc.kind == HIT_DOOR)
	{
		tex_x = (int)(data->rc.wall_x * current_tex->width);
		if (tex_x < 0)
			tex_x = 0;
		else if (tex_x >= current_tex->width)
			tex_x = current_tex->width - 1;
		current_tex->tex_x = tex_x;
	}
	else
		current_tex->tex_x = flip_text_horizontally(data, current_tex);
	draw(data, current_tex, infos);
	color_floor_and_ceiling(data, infos.view_height, infos.start_x, infos.x);
}
