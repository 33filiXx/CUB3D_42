/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 04:41:08 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 17:26:48 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	free_cmp_data(t_cmp_data *cmp)
{
	int	i;

	if (!cmp)
		return ;
	if (cmp->compass)
	{
		i = 0;
		while (cmp->compass[i])
		{
			free(cmp->compass[i]);
			i++;
		}
		free(cmp->compass);
	}
	free(cmp);
}

void	free_double_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_file_data(t_file_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->map)
	{
		i = 0;
		while (data->map[i])
		{
			free(data->map[i]);
			i++;
		}
		free(data->map);
		data->map = NULL;
	}
	free_texture_paths(data);
	free(data);
}

unsigned int	pack_color(int color[3])
{
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;

	red = (unsigned int)(color[0] & 0xFF);
	green = (unsigned int)(color[1] & 0xFF);
	blue = (unsigned int)(color[2] & 0xFF);
	return ((red << 16) | (green << 8) | blue);
}

int	trim_row(char *row)
{
	int	len;

	if (!row)
		return (0);
	len = 0;
	while (row[len] && row[len] != '\n')
	{
		if (row[len] == '+')
			row[len] = '1';
		len++;
	}
	row[len] = '\0';
	return (len);
}

