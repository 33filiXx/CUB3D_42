/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 13:56:38 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static t_texture	g_gun_texture;
static bool			g_gun_loaded = false;

void	gun_init(t_game_data *data)
{
	if (g_gun_loaded)
		return ;
	g_gun_texture.mlx_connection = data->mlx.mlx_connection;
	g_gun_texture.img = mlx_xpm_file_to_image(data->mlx.mlx_connection,
			"textures/gun.xpm", &g_gun_texture.width, &g_gun_texture.height);
	if (!g_gun_texture.img)
	{
		printf("Warning: gun.xpm not found, gun disabled\n");
		return ;
	}
	g_gun_texture.addr = mlx_get_data_addr(g_gun_texture.img,
			&g_gun_texture.bpp, &g_gun_texture.line_len, &g_gun_texture.endian);
	g_gun_texture.transparent_color = texel(&g_gun_texture, 0, 0);
	g_gun_texture.has_transparency = true;
	g_gun_loaded = true;
}

static void	draw_gun_pixel(t_game_data *data, int x, int y, int *tex)
{
	unsigned int	color;
	unsigned int	masked;

	if (tex[0] < 0 || tex[0] >= g_gun_texture.width)
		return ;
	if (tex[1] < 0 || tex[1] >= g_gun_texture.height)
		return ;
	color = texel(&g_gun_texture, tex[0], tex[1]);
	masked = color & 0x00FFFFFF;
	if (masked == (g_gun_texture.transparent_color & 0x00FFFFFF))
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

	if (!g_gun_loaded || !g_gun_texture.addr)
		return ;
	dims[3] = 4;
	dims[2] = g_gun_texture.width * dims[3];
	dims[0] = (WIDTH - dims[2]) / 2 + WIDTH / 5;
	dims[1] = HEIGHT - g_gun_texture.height * dims[3];
	y = 0;
	while (y < g_gun_texture.height * dims[3])
	{
		render_gun_row(data, y, dims);
		y++;
	}
}

void	gun_destroy(void)
{
	if (!g_gun_loaded)
		return ;
	if (g_gun_texture.img && g_gun_texture.mlx_connection)
		mlx_destroy_image(g_gun_texture.mlx_connection, g_gun_texture.img);
	g_gun_texture.img = NULL;
	g_gun_texture.addr = NULL;
	g_gun_loaded = false;
}
