/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 21:05:08 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 21:17:23 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"


static void free_sprite_array(t_game_data *data)
{
	if (!data)
		return ;
	if (data->sprite)
		free(data->sprite);
	data->sprite = NULL;
	data->sprite_count = 0;
	data->v_sprite_counter = 0;
}

static void free_door_array(t_game_data *data)
{
	if (!data)
		return ;
	if (data->doors)
		free(data->doors);
	data->doors = NULL;
	data->door_count = 0;
}

static void free_z_buffer_data(t_game_data *data)
{
	if (!data)
		return ;
	if (data->z_buffer)
		free(data->z_buffer);
	data->z_buffer = NULL;
	data->z_buffer_size = 0;
}



void	cleanup_game(t_game_data *data)
{
	if (!data)
		return ;
	free_sprite_array(data);
	free_door_array(data);
	free_z_buffer_data(data);
	free_texture_paths(&data->file_data);
	destroy_textures(data);
	sprite_sheet_destroy();
	destroy_mlx_resources(&data->mlx);
}
