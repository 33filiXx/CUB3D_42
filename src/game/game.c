/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:04:52 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/03 19:05:50 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	initiate(t_mlx *mlx, t_game_data *game_data)
{
	(void)game_data;
	mlx->mlx_connection = mlx_init();
	if (!mlx->mlx_connection)
		exit(1);
	mlx->mlx_win = mlx_new_window(mlx->mlx_connection, WIDTH, HEIGHT, "Cube3D");
	if (!mlx->mlx_win)
	{
		mlx_destroy_display(mlx->mlx_connection);
		free(mlx->mlx_connection);
		exit(1);
	}
	mlx->img = mlx_new_image(mlx->mlx_connection, WIDTH, HEIGHT);
	if (!mlx->img)
	{
		mlx_destroy_display(mlx->mlx_connection);
		free(mlx->mlx_connection);
		exit(1);
	}
	mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bits_per_pixel,
			&mlx->line_length, &mlx->endian);
	printf("framebuffer bpp=%d endian=%d line_len=%d\n", mlx->bits_per_pixel,
		mlx->endian, mlx->line_length);
}

void	redraw_map(t_game_data *data)
{
	render_3d_view(data, 0, WIDTH, HEIGHT);
	sprite_render_all(data, 0, WIDTH, HEIGHT);
	draw_env(data);
	mlx_put_image_to_window(data->mlx.mlx_connection, data->mlx.mlx_win,
		data->mlx.img, 0, 0);
}

int	game_loop(void *param)
{
	t_game_data	*data;
	double		now;
	double		dt;
	bool		moved;

	data = (t_game_data *)param;
	now = get_now_seconds();
	dt = now - data->last_time;
	moved = false;
	data->last_time = now;
	door_update(data, dt);
	sprite_update_all(data, dt);
	set_moved_flag(data, &moved);
	if (moved || data->sprite_count > 0)
		redraw_map(data);
	return (0);
}

int	close_window(void *param)
{
	t_game_data	*data;

	data = (t_game_data *)param;
	cleanup_game(data);
	exit(0);
	return (0);
}
