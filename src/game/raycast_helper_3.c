/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_helper_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:29:34 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/01 18:13:39 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	set_drawing_ends(t_game_data *data, int view_height)
{
	data->rc.draw_start = -data->rc.line_height / 2 + view_height / 2;
	if (data->rc.draw_start < 0)
		data->rc.draw_start = 0;
	data->rc.draw_end = data->rc.line_height / 2 + view_height / 2;
	if (data->rc.draw_end >= view_height)
		data->rc.draw_end = view_height - 1;
}

void	set_texture_coordinations(t_game_data *data)
{
	if (data->rc.kind == HIT_DOOR)
		return ;
	if (data->rc.side == 0) // vertical wall
		data->rc.wall_x = data->player.pos.y + data->rc.perp_wall_dist
			* data->rc.ray_dir_y;
	else // horizontal wall
		data->rc.wall_x = data->player.pos.x + data->rc.perp_wall_dist
			* data->rc.ray_dir_x;
	// Keep only the fractional part within [0, 1)
	data->rc.wall_x -= floor(data->rc.wall_x);
}

char	*get_the_right_texture(t_game_data *data)
{
	char	*texture_path;

	if (data->rc.side == 0) // vertical wall
	{
		if (data->rc.ray_dir_x > 0)
			texture_path = data->file_data.ea_texture; // facing EAST
		else
			texture_path = data->file_data.we_texture; // facing WEST
	}
	else // horizontal wall
	{
		if (data->rc.ray_dir_y > 0)
			texture_path = data->file_data.so_texture; // facing SOUTH
		else
			texture_path = data->file_data.no_texture; // facing NORTH
	}
	return (texture_path);
}

void	load_texture(t_game_data *data, t_texture *tex, char *path)
{
	(void)data;
	tex->img = mlx_xpm_file_to_image(tex->mlx_connection, path, &tex->width,
			&tex->height);
	if (!tex->img)
		(printf("Texture load failed : %s\n", path), exit(1));
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
	tex->transparent_color = 0;
	tex->has_transparency = false;
}

t_texture	*get_current_texture(t_game_data *data, t_texture *tex_no,
		t_texture *tex_so, t_texture *tex_we, t_texture *tex_ea)
{
	if (data->rc.side == 0) // vertical wall
	{
		if (data->rc.ray_dir_x > 0)
			return (tex_ea); // looking EAST
		else
			return (tex_we); // looking WEST
	}
	else // horizontal wall
	{
		if (data->rc.ray_dir_y > 0)
			return (tex_so); // looking SOUTH
		else
			return (tex_no); // looking NORTH
	}
}
