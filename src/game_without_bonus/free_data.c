/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 21:05:08 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/03 22:51:14 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	free_map_grid(t_game_data *data)
{
	int	i;

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
	free_map_grid(data);
	free_texture_paths(&data->file_data);
	destroy_textures(data);
	destroy_mlx_resources(&data->mlx);
}
