/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/09 14:00:00 by rhafidi          ###   ########.fr       */
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

static unsigned int	get_gun_pixel(int tex_x, int tex_y)
{
	if (tex_x < 0 || tex_x >= g_gun_texture.width)
		return (0);
	if (tex_y < 0 || tex_y >= g_gun_texture.height)
		return (0);
	return (texel(&g_gun_texture, tex_x, tex_y));
}

static void	draw_gun_pixel(t_game_data *data, int x, int y, unsigned int color)
{
	unsigned int	masked;

	masked = color & 0x00FFFFFF;
	if (masked == (g_gun_texture.transparent_color & 0x00FFFFFF))
		return ;
	if (masked == 0)
		return ;
	put_pixel(&data->mlx, x, y, color);
}

void	gun_render(t_game_data *data)
{
	int		x;
	int		y;
	int		start_x;
	int		start_y;

	if (!g_gun_loaded || !g_gun_texture.addr)
		return ;
	start_x = (WIDTH - g_gun_texture.width) / 2;
	start_y = HEIGHT - g_gun_texture.height;
	y = 0;
	while (y < g_gun_texture.height)
	{
		x = 0;
		while (x < g_gun_texture.width)
		{
			draw_gun_pixel(data, start_x + x, start_y + y,
				get_gun_pixel(x, y));
			x++;
		}
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
