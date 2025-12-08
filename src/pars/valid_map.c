/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 15:17:19 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/08 17:49:56 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	drain_fd(int fd)
{
	char	*line;

	while ((line = get_next_line(fd)) != NULL)
		free(line);
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
	// map_info[8] = '+';
	map_info[8] = ' ';
	map_info[9] = '\0';
}
void skip_plus(char *line , int *j)
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
			if(s1 == '1' || s1 == '0' || s1 == 'X' || s1 == 'D' || s1 == ' ')
				return 0;
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
int last_floor(char *line , int j)
{
	int i ;

	i = j + 1;
	while (line[i] && line[i] != '+' && line[i] != '\n')
	{
		if (line[i] == '0')
			return 1;
		i++;
	}
	return 0;
}

int is_door_valid(char *str , char *next , char *prev)
{
	int i;

	i =  0;
	while (str[i])
	{
		if (str[i] == 'D')
		{
			if ((str[i - 1] == '1' && str[i + 1] == '1') || (next[i] == '1' && prev[i] == '1'))
				return 0;
			else
				return 1;
		}
		i++;
	}
	return 0;
}




int is_valid(t_file_data *file_data)
{
	int i;
	int j;
	int start;

	i = 0;
	while (file_data->map[i])
	{
		j = 0;
		skip_plus(file_data->map[i] , &j);
		start = j;
		while (file_data->map[i][j] && file_data->map[i][j] != '\n')
		{
			if (file_data->map[0][j] != '1' && file_data->map[0][j] != '+' && file_data->map[0][j] != ' ')
				return 1;
			if (file_data->map[i][start] != '1')
					return 1;
			if (i >= 1 && is_door_valid(file_data->map[i] , file_data->map[i + 1] , file_data->map[i + 1]))
				return 1;
			if(file_data->map[i + 1] && i >= 1 && file_data->map[i][j] == '0')
			{
				if(file_data->map[i][j + 1] == '+' || file_data->map[i][j + 1] == '\n')
					return 1;
				if(file_data->map[i - 1][j] == '+' || file_data->map[i + 1][j] == '+' || file_data->map[i - 1][j] == ' ' || file_data->map[i + 1][j] == ' ')
					return 1;
			}
			if(!file_data->map[i + 1])
			{
				if(file_data->map[i][j] != '1' && file_data->map[i][j] != '+')
					return 1;
			}
			j++;
		}
		i++;
	}
	return 0;
}





int	storing(int fd, t_file_data *file_data)
{
	char	*map_info;
	int		i;
	int		j;
	int		value;
	int		*checked;
	map_info = malloc(11 * sizeof(char));
	value = 0;
	checked = &value;
	reset_map_info(map_info);
	i = 0;

	if (set_data(fd, file_data))
	{
		drain_fd(fd);
		free(map_info);
		close(fd);
		return (1);
	}

	while (file_data->map[i])
	{
		j = 0;
		skip_plus(file_data->map[i] , &j);
		while (file_data->map[i][j] && file_data->map[i][j] != '\n')
		{
			if (!(*checked) && found_player(file_data->map[i][j], map_info ,checked))
			{
				file_data->row = i;
				file_data->column = j;
			}
			else if ((found_player(file_data->map[i][j], map_info ,checked) && *checked) || *checked == 2)
			{
				//printf("%s\n" , file_data->map[i]);
				drain_fd(fd);
				free(map_info);
				close(fd);
				return (1);
			}
			j++;
		}
		if (j < file_data->s_element_size - 1)
			add_characters(file_data->map[i], file_data, j);
		i++;
	}
	if(is_valid(file_data))
	{
		drain_fd(fd);
		free(map_info);
		close(fd);
		return 1;
	}
	if (!(*checked))
	{
		drain_fd(fd);
		free(map_info);
		close(fd);
		return 1;
	}
	close(fd);
	free(map_info);
	return (0);
}
