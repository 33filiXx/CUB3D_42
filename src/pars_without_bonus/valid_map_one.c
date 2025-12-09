/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 02:05:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 04:23:55 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	drain_fd(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		line = get_next_line(fd);
	}
}

void	reset_map_info(char *map_info)
{
	map_info[0] = '0';
	map_info[1] = '1';
	map_info[2] = 'W';
	map_info[3] = 'N';
	map_info[4] = 'S';
	map_info[5] = 'E';
	map_info[6] = 'D';
	map_info[7] = 'X';
	map_info[8] = ' ';
	map_info[9] = '\0';
}

void	skip_plus(char *line, int *j)
{
	while (line[*j] && line[*j] == '+')
		(*j)++;
}

int	found_player(char s1, char *s2, int *checked)
{
	int	i;

	i = 0;
	while (s2[i])
	{
		if (s1 == s2[i])
		{
			if (s1 == '1' || s1 == '0' || s1 == 'X' || s1 == 'D' || s1 == ' ')
				return (0);
			*checked = 1;
			return (1);
		}
		i++;
	}
	*checked = 2;
	return (0);
}

void	add_characters(char *str, t_file_data *file_data, int j)
{
	int	i;

	i = j;
	while (i < file_data->s_element_size - 2)
		str[i++] = '+';
	str[i++] = '\n';
	str[i] = '\0';
}
