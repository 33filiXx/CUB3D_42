/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 21:05:08 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/27 18:19:41 by wel-mjiy         ###   ########.fr       */
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

static void free_map_grid(t_game_data *data)
{
	int i;

	if (!data || !data->map.grid)
		return ;
	i = 0;
	while (i < data->map.height && data->map.grid[i])
	{
		free(data->map.grid[i]);
		data->map.grid[i] = NULL;
		i++;
	}
	free(data->map.grid);
	data->map.grid = NULL;
	data->file_data.map = NULL;
	data->map.width = 0;
	data->map.height = 0;
}



void	cleanup_game(t_game_data *data)
{
	if (!data)
		return ;
	free_sprite_array(data);
	free_door_array(data);
	free_z_buffer_data(data);
	free_map_grid(data);
	free_texture_paths(&data->file_data);
	destroy_textures(data);
	sprite_sheet_destroy();
	destroy_mlx_resources(&data->mlx);
}
