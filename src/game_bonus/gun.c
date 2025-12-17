/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/17 18:02:30 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	gun_init(t_game_data *data)
{
	if (data->gun_loaded)
		return ;
	data->gun_texture.mlx_connection = data->mlx.mlx_connection;
	data->gun_texture.img = mlx_xpm_file_to_image(data->mlx.mlx_connection,
			"textures/gun.xpm", &data->gun_texture.width,
			&data->gun_texture.height);
	if (!data->gun_texture.img)
	{
		printf("Warning: gun.xpm not found, gun disabled\n");
		return ;
	}
	data->gun_texture.addr = mlx_get_data_addr(data->gun_texture.img,
			&data->gun_texture.bpp, &data->gun_texture.line_len,
			&data->gun_texture.endian);
	data->gun_texture.transparent_color = texel(&data->gun_texture, 0, 0);
	data->gun_texture.has_transparency = true;
	data->gun_loaded = true;
}

static void	draw_gun_pixel(t_game_data *data, int x, int y, int *tex)
{
	unsigned int	color;
	unsigned int	masked;

	if (tex[0] < 0 || tex[0] >= data->gun_texture.width)
		return ;
	if (tex[1] < 0 || tex[1] >= data->gun_texture.height)
		return ;
	color = texel(&data->gun_texture, tex[0], tex[1]);
	masked = color & 0x00FFFFFF;
	if (masked == (data->gun_texture.transparent_color & 0x00FFFFFF))
		return ;
	if (masked == 0)
		return ;
	put_pixel(&data->mlx, x, y, color);
}

static void	render_gun_row(t_game_data *data, int y, int *dims)
{
	int	x;
	int	tex[2];

	x = 0;
	while (x < dims[2])
	{
		tex[0] = x / dims[3];
		tex[1] = y / dims[3];
		draw_gun_pixel(data, dims[0] + x, dims[1] + y, tex);
		x++;
	}
}

void	gun_render(t_game_data *data)
{
	int				y;
	int				dims[4];

	if (!data->gun_loaded || !data->gun_texture.addr)
		return ;
	dims[3] = 4;
	dims[2] = data->gun_texture.width * dims[3];
	dims[0] = (WIDTH - dims[2]) / 2 + WIDTH / 5;
	dims[1] = HEIGHT - data->gun_texture.height * dims[3];
	y = 0;
	while (y < data->gun_texture.height * dims[3])
	{
		render_gun_row(data, y, dims);
		y++;
	}
}

void	gun_destroy(t_game_data *data)
{
	if (!data->gun_loaded)
		return ;
	if (data->gun_texture.img && data->gun_texture.mlx_connection)
		mlx_destroy_image(data->gun_texture.mlx_connection,
			data->gun_texture.img);
	data->gun_texture.img = NULL;
	data->gun_texture.addr = NULL;
	data->gun_loaded = false;
}
