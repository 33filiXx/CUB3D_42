/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:34:54 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/11/27 21:05:18 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

// static void	free_map_partial(t_file_data *file_data, int filled)
// {
// 	int i;

// 	if (!file_data || !file_data->map)
// 		return;
// 	i = 0;
// 	while (i < filled)
// 	{
// 		if (file_data->map[i])
// 			free(file_data->map[i]);
// 		i++;
// 	}
// 	free(file_data->map);
// }

static char *dup_trimmed_token(char *token)
{
	char	*trimmed;
	char	*result;

	if (!token)
		return (NULL);
	trimmed = ft_strtrim(token, " \t\n\r");
	if (!trimmed)
		return (NULL);
	result = ft_strdup(trimmed);
	free(trimmed);
	return (result);
}

void cleanup_inside_set_data(t_cmp_data *cmp_data, char **already_checked, char **to_be_splited, char *buffer)
{
	if (cmp_data)
		free_cmp_data(cmp_data);
	if (already_checked)
		free_double_array(already_checked);
	if (to_be_splited)
		free_double_array(to_be_splited);
	free(buffer);
}

static void reset_data(t_cmp_data *cmp_data)
{
	cmp_data->compass = malloc(7 * sizeof(char *));
	cmp_data->compass[0] = ft_strdup(NO);
	cmp_data->compass[1] = ft_strdup(SO);
	cmp_data->compass[2] = ft_strdup(WE);
	cmp_data->compass[3] = ft_strdup(EA);
	cmp_data->compass[4] = ft_strdup(F);
	cmp_data->compass[5] = ft_strdup(C);
	cmp_data->compass[6] = NULL;
}

int match_in_list(char *s1, char **s2, char **already_checked)
{
	int i;
	int j;
	int checked;

	j = 0;
	checked = 0;
	if (!s1 || !s2)
		return (1);
	while (s2[j])
	{
		i = 0;
		while (s1 && s2[j] && s1[i] && s2[j][i] && s1[i] == s2[j][i])
			i++;
		if (!(s1[i] - s2[j][i]))
		{
			if (!already_checked[checked])
			{
				already_checked[checked] = ft_strdup(s1);
				return (0);
			}
			while (already_checked[checked])
			{
				if (!ft_strcmp(s1, already_checked[checked]))
					return (2);
				checked++;
			}
			already_checked[checked] = ft_strdup(s1);
			return (0);
		}
		j++;
	}
	return (1);
}
int comma_length_checker(char *str)
{
	int i;
	int counter;
	int comma;

	i = 0;
	counter = 0;
	comma = 0;
	while (str[i])
	{
		if (str[i] == ',' && str[i + 1] && str[i + 1] == ',')
			return (1);
		i++;
	}
	i = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			counter++;
			if (counter > 3)
			{
				return (1);
			}
		}
		else if (str[i] == ',')
		{
			counter = 0;
			comma++;
		}
		if (comma > 2)
			return (1);
		i++;
	}
	return (0);
}

int next_one(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			return 1;
		else if ((str[i] >= '0' && str[i] <= '9'))
			return 0;
		i++;
	}
	return 0;
}

int specific_store(t_file_data *file_data, char who_know, char *buffer)
{
	int i;
	char *tmp;
	int j;
	int p;
	int check;

	i = 1;
	j = 0;
	p = 0;
	check = 0;
	tmp = malloc(4);
	while (buffer[i] && buffer[i] != '\n')
	{
		if (who_know == 'F')
		{
			if (buffer[i] >= '0' && buffer[i] <= '9')
			{
				if (p >= 3)
				{
					free(tmp);
					return 1;
				}
				tmp[p++] = buffer[i];
				check = 1;
			}
			else if (buffer[i] == ',')
			{
				tmp[p] = '\0';
				check = 0;
				if (ft_atoi(tmp) > 255)
				{
					free(tmp);
					return 1;
				}
				file_data->floor_color[j++] = ft_atoi(tmp);
				free(tmp);
				tmp = malloc(4);
				i++;
				p = 0;
				continue;
			}
			else if (buffer[i] != ' ')
			{
				free(tmp);
				return 1;
			}
			if (check && buffer[i] == ' ' && !next_one(buffer + i))
			{
				free(tmp);
				return 1;
			}
		}
		else if (who_know == 'C')
		{
			if (buffer[i] >= '0' && buffer[i] <= '9')
			{
				if (p >= 3)
				{
					free(tmp);
					return 1;
				}
				tmp[p++] = buffer[i];
				check = 1;
			}
			else if (buffer[i] == ',')
			{
				tmp[p] = '\0';
				check = 0;
				if (ft_atoi(tmp) > 255)
				{
					free(tmp);
					return 1;
				}
				file_data->ceiling_color[j++] = ft_atoi(tmp);
				free(tmp);
				tmp = malloc(4);
				i++;
				p = 0;
				continue;
			}
			else if (buffer[i] != ' ')
			{
				free(tmp);
				return 1;
			}
			if (check && buffer[i] == ' ' && !next_one(buffer + i))
			{
				free(tmp);
				return 1;
			}
		}
		i++;
	}
	if (check && p > 0)
	{
		tmp[p] = '\0';
		if (who_know == 'F')
		{
			if (ft_atoi(tmp) > 255)
			{
				free(tmp);
				return 1;
			}
			if (j < 3)
				file_data->floor_color[j++] = ft_atoi(tmp);
		}
		else if (who_know == 'C')
		{
			if (ft_atoi(tmp) > 255)
			{
				free(tmp);
				return 1;
			}
			if (j < 3)
				file_data->ceiling_color[j++] = ft_atoi(tmp);
		}
	}
	free(tmp);
	return (0);
}
int check_if_exact(char **str)
{
	if (str[2])
		return 1;
	else	
		return 0;
}
int store_in_the_right_place(char **to_be_splited, t_file_data *file_data, char *buffer)
{
	if (!strcmp(to_be_splited[0], NO) )
	{
		if( check_if_exact(to_be_splited))
			return 1;
		file_data->no_texture = dup_trimmed_token(to_be_splited[1]);
		if (!file_data->no_texture)
			return 1;
	}
	else if (!strcmp(to_be_splited[0], SO) )
	{
		if( check_if_exact(to_be_splited))
			return 1;
		file_data->so_texture = dup_trimmed_token(to_be_splited[1]);
		if (!file_data->so_texture)
			return 1;
	}
	else if (!strcmp(to_be_splited[0], WE) )
	{
		if( check_if_exact(to_be_splited))
			return 1;
		file_data->we_texture = dup_trimmed_token(to_be_splited[1]);
		if (!file_data->we_texture)
			return 1;
	}
	else if (!strcmp(to_be_splited[0], EA) )
	{
		if( check_if_exact(to_be_splited))
			return 1;
		file_data->ea_texture = dup_trimmed_token(to_be_splited[1]);
		if (!file_data->ea_texture)
			return 1;
	}
	else if (!strcmp(to_be_splited[0], F))
	{
		if (!comma_length_checker(buffer))
		{
			if (specific_store(file_data, 'F', ft_strchr(buffer, 'F')))
				return 1;
		}
		else
			return (1);
	}
	else if (!strcmp(to_be_splited[0], C))
	{
		if (!comma_length_checker(buffer))
		{
			if (specific_store(file_data, 'C', ft_strchr(buffer, 'C')))
				return (1);
		}
		else
			return (1);
	}
	return (0);
}

int is_empty(char **arry)
{
	int i;

	i = 0;
	while (arry[i])
		i++;
	if (i < 6)
		return (1);
	return (0);
}

int is_color_dup(t_file_data *file_data)
{
	int i;
	int check;

	i = 0;
	check = 0;
	while (file_data->floor_color[i])
	{
		if (file_data->floor_color[i] == file_data->ceiling_color[i])
			check++;
		i++;
	}
	if (check == 3)
		return (1);
	return (0);
}

int is_only_space(char *str)
{
	int i;
	int just_space;

	i = 0;
	just_space = 1;
	if (!str)
	{
		return 1;
	}
	while (str[i])
	{
		if (str[i] != ' ' || str[i] != '\n')
		{
			just_space = 0;
		}
		i++;
	}
	return just_space;
}

void update_line(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			line[i] = '1';
		i++;
	}
}

int skip_empty_line(char *line)
{
	int i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ' || line[i] != '\t')
			return 0;
		i++;
	}
	return 1;
}
void fill_map(char *line, t_file_data *file_data, int *update_map_arr)
{
	int i;

	i = 0;
	file_data->map[*update_map_arr] = malloc(file_data->element_size);
	if (skip_empty_line(line) && *update_map_arr == 0)
	{
		free(file_data->map[*update_map_arr]);
		return;
	}
	while (line[i])
	{
		update_line(line);
		if (line && ft_strlen(line) > file_data->s_element_size)
			file_data->s_element_size = ft_strlen(line);
		file_data->map[*update_map_arr][i] = line[i];
		i++;
	}
	file_data->map[*update_map_arr][i] = '\0';
	(*update_map_arr)++;
}

int set_data(int fd, t_file_data *file_data)
{
	char *buffer;
	char **to_be_splited;
	t_cmp_data *cmp_data;
	char **already_checked;
	int value;
	int *update_map_arr;
	int fill_only_map;

	cmp_data = malloc(sizeof(t_cmp_data));
	reset_data(cmp_data);
	value = 0;
	update_map_arr = &value;
	fill_only_map = 0;
	already_checked = ft_calloc(7, sizeof(char *));
	file_data->map = ft_calloc(file_data->map_size + 1, sizeof(char *));
	while (1)
	{
		buffer = get_next_line(fd);
		if (buffer)
		{
			to_be_splited = ft_split(buffer, ' ');
			if (!match_in_list(to_be_splited[0], cmp_data->compass,
							   already_checked))
			{
				if (store_in_the_right_place(to_be_splited, file_data, buffer))
				{
					cleanup_inside_set_data(cmp_data, already_checked, to_be_splited, buffer);
					return (1);
				}
			}
			else if (match_in_list(to_be_splited[0], cmp_data->compass,
								   already_checked) == 2)
			{
				cleanup_inside_set_data(cmp_data, already_checked, to_be_splited, buffer);
				return (1);
			}
			if (buffer && !is_empty(already_checked) && fill_only_map)
				fill_map(buffer, file_data, update_map_arr);
			if (!is_empty(already_checked))
				fill_only_map = 1;
			free_double_array(to_be_splited);
			free(buffer);
		}
		else
			break;
	}
	file_data->map[value] = NULL;
	if (is_empty(already_checked) || is_color_dup(file_data))
	{
		free_cmp_data(cmp_data);
		free_double_array(already_checked);
		return (1);
	}
	file_data->s_element_size++;
	free_cmp_data(cmp_data);
	free_double_array(already_checked);
	return (0);
}
