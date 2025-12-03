/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_helper_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 21:21:20 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/03 19:25:11 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_vec2	ray_origin(t_game_data *data)
{
	return (data->player.pos);
}

t_vec2	ray_direction(t_game_data *data)
{
	return (vec2_new(data->rc.ray_dir_x, data->rc.ray_dir_y));
}

void	set_origin_dir(t_game_data *data, t_vec2 *origin, t_vec2 *dir)
{
	*origin = ray_origin(data);
	*dir = ray_direction(data);
}

t_infos	get_infos(int start_x, int x, int view_height)
{
	t_infos	infos;

	infos.start_x = start_x;
	infos.view_height = view_height;
	infos.x = x;
	infos.view_width = 0;
	return (infos);
}
