/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 15:17:19 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 02:26:36 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int is_valid(t_file_data *file_data)
{
	t_helper_data *helper_data;

	helper_data = malloc(sizeof(t_helper_data));
	reset_data_helper(helper_data, 0);
	while (file_data->map[helper_data->i])
	{
		helper_data->j = 0;
		skip_plus(file_data->map[helper_data->i], &(helper_data->j));
		helper_data->start = helper_data->j;
		while (file_data->map[helper_data->i][helper_data->j] && file_data->map[helper_data->i][helper_data->j] != '\n')
		{
			if (is_valid_helper(file_data, helper_data))
			{
				free(helper_data);
				return 1;
			}
			(helper_data->j)++;
		}
		(helper_data->i)++;
	}
	free(helper_data);
	return 0;
}

int cleanup_and_exit(int fd, char *map_info, int ret)
{
	drain_fd(fd);
	free(map_info);
	close(fd);
	return (ret);
}

int process_char(t_file_data *file_data, char *map_info,
						int *checked, int i, int j)
{
	if (!(*checked) && found_player(file_data->map[i][j],
									map_info, checked))
	{
		file_data->row = i;
		file_data->column = j;
	}
	else if ((found_player(file_data->map[i][j], map_info, checked) && *checked) || *checked == 2)
		return (1);
	return (0);
}

int scan_rows(t_file_data *file_data, char *map_info, int *checked)
{
	int i;
	int j;

	i = 0;
	while (file_data->map[i])
	{
		j = 0;
		skip_plus(file_data->map[i], &j);
		while (file_data->map[i][j] && file_data->map[i][j] != '\n')
		{
			if (process_char(file_data, map_info, checked, i, j))
				return (1);
			j++;
		}
		if (j < file_data->s_element_size - 1)
			add_characters(file_data->map[i], file_data, j);
		i++;
	}
	return (0);
}

int storing(int fd, t_file_data *file_data)
{
	char *map_info;

	int value;
	int *checked;
	map_info = malloc(11 * sizeof(char));
	value = 0;
	checked = &value;
	reset_map_info(map_info);
	if (set_data(fd, file_data))
		return (cleanup_and_exit(fd, map_info, 1));
	if (scan_rows(file_data, map_info, checked))
		return (cleanup_and_exit(fd, map_info, 1));
	if (is_valid(file_data))
		return (cleanup_and_exit(fd, map_info, 1));
	if (!(*checked))
		return (cleanup_and_exit(fd, map_info, 1));
	close(fd);
	free(map_info);
	return (0);
}
