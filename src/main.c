/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:19:29 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 17:29:22 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	main(int ac, char **av)
{
	t_game_data (game_data);
	if (ac != 2)
		return (report_error("Usage: ./cub3D <map.cub>"));
	ft_bzero(&game_data, sizeof(t_game_data));
	if (parse_and_merge(&game_data, av))
		return (1);
	initiate(&game_data.mlx, &game_data);
	door_load_map(&game_data);
	sprite_load_map(&game_data);
	sprite_update_all(&game_data, 0.0);
	redraw_map(&game_data);
	mlx_hook(game_data.mlx.mlx_win, KeyPress, KeyPressMask, key_press,
		&game_data);
	mlx_hook(game_data.mlx.mlx_win, KeyRelease, KeyReleaseMask, key_release,
		&game_data);
	mlx_loop_hook(game_data.mlx.mlx_connection, game_loop, &game_data);
	mlx_hook(game_data.mlx.mlx_win, DestroyNotify, StructureNotifyMask,
		close_window, &game_data);
	mlx_put_image_to_window(game_data.mlx.mlx_connection, game_data.mlx.mlx_win,
		game_data.mlx.img, 0, 0);
	init_mouse(&game_data);
	mlx_hook(game_data.mlx.mlx_win, MotionNotify, PointerMotionMask,
		on_mouse_move, &game_data);
	mlx_loop(game_data.mlx.mlx_connection);
	return (cleanup_game(&game_data), 0);
}
