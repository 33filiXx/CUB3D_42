/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 21:16:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 21:19:58 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"



void destroy_mlx_resources(t_mlx *mlx)
{
	if (!mlx)
		return ;
	if (mlx->img && mlx->mlx_connection)
	{
		mlx_destroy_image(mlx->mlx_connection, mlx->img);
		mlx->img = NULL;
	}
	if (mlx->mlx_win && mlx->mlx_connection)
	{
		mlx_destroy_window(mlx->mlx_connection, mlx->mlx_win);
		mlx->mlx_win = NULL;
	}
	if (mlx->mlx_connection)
	{
		mlx_destroy_display(mlx->mlx_connection);
		free(mlx->mlx_connection);
		mlx->mlx_connection = NULL;
	}
}

void free_texture_paths(t_file_data *file_data)
{
	if (!file_data)
		return ;
	free(file_data->no_texture);
	file_data->no_texture = NULL;
	free(file_data->so_texture);
	file_data->so_texture = NULL;
	free(file_data->we_texture);
	file_data->we_texture = NULL;
	free(file_data->ea_texture);
	file_data->ea_texture = NULL;
}