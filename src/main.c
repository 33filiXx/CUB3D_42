/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:19:29 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/10 16:05:47 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	setup_hooks(t_game_data *data)
{
	mlx_hook(data->mlx.mlx_win, KeyPress, KeyPressMask, key_press, data);
	mlx_hook(data->mlx.mlx_win, KeyRelease, KeyReleaseMask, key_release, data);
	mlx_loop_hook(data->mlx.mlx_connection, game_loop, data);
	mlx_hook(data->mlx.mlx_win, DestroyNotify, StructureNotifyMask,
		close_window, data);
	init_mouse(data);
	mlx_hook(data->mlx.mlx_win, MotionNotify, PointerMotionMask,
		on_mouse_move, data);
}

static void	init_game(t_game_data *data)
{
	initiate(&data->mlx, data);
	gun_init(data);
	door_load_map(data);
	sprite_load_map(data);
	sprite_update_all(data, 0.0);
	redraw_map(data);
}

int	main(int ac, char **av)
{
	t_game_data	game_data;

	if (ac != 2)
		return (report_error("Usage: ./cub3D <map.cub>"));
	ft_bzero(&game_data, sizeof(t_game_data));
	if (parse_and_merge(&game_data, av))
		return (1);
	init_game(&game_data);
	setup_hooks(&game_data);
	mlx_put_image_to_window(game_data.mlx.mlx_connection, game_data.mlx.mlx_win,
		game_data.mlx.img, 0, 0);
	mlx_loop(game_data.mlx.mlx_connection);
	return (cleanup_game(&game_data), 0);
}
