/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:40:21 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 20:40:46 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void rotate_player(t_game_data *data, double angle)
{
    double old_dir_x = data->player.dir.x;
    double old_plane_x = data->player.plane.x;

    data->player.dir.x = old_dir_x * cos(angle) - data->player.dir.y * sin(angle);
    data->player.dir.y = old_dir_x * sin(angle) + data->player.dir.y * cos(angle);
    data->player.plane.x = old_plane_x * cos(angle) - data->player.plane.y * sin(angle);
    data->player.plane.y = old_plane_x * sin(angle) + data->player.plane.y * cos(angle);
}
