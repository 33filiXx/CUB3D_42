/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_helper_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 21:21:20 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 21:21:29 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_vec2   ray_origin(t_game_data *data)
{
    return (data->player.pos);
}

t_vec2   ray_direction(t_game_data *data)
{
    return (vec2_new(data->rc.ray_dir_x, data->rc.ray_dir_y));
}