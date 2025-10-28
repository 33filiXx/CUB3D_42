/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:34:51 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/10/28 10:38:15 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	reset_map_info(char *map_info)
{
	
	map_info[0] = '0';
	map_info[1] = '1';
	map_info[2] = 'W';
	map_info[3] = 'N';
	map_info[4] = 'S';
	map_info[5] = 'E';
	map_info[6] = '\0';
}
int	found_player(char s1, char *s2, int *checked)
{
	int	i;

	i = 0;
	while (s2[i])
	{
		if (s1 == s2[i])
		{
			if(s1 == '1' || s1 == '0')
				return 0;
			*checked = 1;
			return (1);
		}
		i++;
	}
	return (3);
}

void	add_characters(char *str, t_file_data *file_data, int j)
{
	int	i;

	i = j;
	while (i < file_data->s_element_size - 1)
		str[i++] = '!';
	str[i] = '\n';
}

int	is_valid(int fd, t_file_data *file_data)
{
	char	*map_info;
	int		i;
	int		j;
	int		spawning;
	int		value;
	int		*checked;
	// int		bool;

	map_info = malloc(8 * sizeof(char));
	value = 0;
	checked = &value;
	reset_map_info(map_info);
	spawning = 0;
	i = 0;
	if (set_data(fd, file_data))
		return (1);
	while (file_data->map[i])
	{
		j = 0;
		while (file_data->map[i][j] && file_data->map[i][j] != '\n')
		{
			// bool = found_player(file_data->map[i][j], map_info ,checked);
			if (*checked == 0 && found_player(file_data->map[i][j], map_info ,checked))
			{
				file_data->row = i;
				file_data->column = j;
			}
			else if (found_player(file_data->map[i][j], map_info ,checked) && *checked)
				return (1);
			if (found_player(file_data->map[i][j], map_info ,checked) == 3)
				return (1);
			j++;
		}
		if (j < file_data->s_element_size - 1)
			add_characters(file_data->map[i], file_data, j);
		i++;
	}
	return (0);
}
