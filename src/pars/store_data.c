/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:34:54 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/10/23 03:13:37 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	reset_data(t_cmp_data *cmp_data)
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

int	match_in_list(char *s1, char **s2, char **already_checked)
{
	int	i;
	int	j;
	int	checked;

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
int	comma_length_checker(char *str)
{
	int	i;
	int	counter;
	int	comma;

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
		if (str[i] != ',' && str[i] != '\n')
		{
			counter++;
			if (counter > 3)
				return (1);
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

int	specific_store(char *str, t_file_data *file_data, char who_know)
{
	int		i;
	int		j;
	int		array_length;
	char	*tmp;

	i = 0;
	j = i;
	array_length = 0;
	tmp = malloc(3);
	while (str[i])
	{
		if (who_know == 'F')
		{
			while (str[i] != ',' && str[i])
				tmp[j++] = str[i++];
			tmp[j] = '\0';
			j = 0;
			file_data->floor_color[array_length++] = ft_atoi(tmp);
			i++;
			continue ;
		}
		else if (who_know == 'C')
		{
			while (str[i] != ',' && str[i])
				tmp[j++] = str[i++];
			tmp[j] = '\0';
			j = 0;
			file_data->ceiling_color[array_length++] = ft_atoi(tmp);
			if (ft_atoi(tmp) > 255 || ft_atoi(tmp) < 0)
				return (1);
			i++;
			continue ;
		}
	}
	free(tmp);
	return (0);
}

int	store_in_the_right_place(char **to_be_splited, t_file_data *file_data)
{
	if (!strcmp(to_be_splited[0], NO))
		file_data->no_texture = ft_strdup(to_be_splited[1]);
	else if (!strcmp(to_be_splited[0], SO))
		file_data->so_texture = ft_strdup(to_be_splited[1]);
	else if (!strcmp(to_be_splited[0], WE))
		file_data->we_texture = ft_strdup(to_be_splited[1]);
	else if (!strcmp(to_be_splited[0], EA))
		file_data->ea_texture = ft_strdup(to_be_splited[1]);
	else if (!strcmp(to_be_splited[0], F))
	{
		if (!comma_length_checker(to_be_splited[1]))
			specific_store(to_be_splited[1], file_data, 'F');
		else
			return (1);
	}
	else if (!strcmp(to_be_splited[0], C))
	{
		if (!comma_length_checker(to_be_splited[1]))
		{
			if (specific_store(to_be_splited[1], file_data, 'C'))
				return (1);
		}
		else
			return (1);
	}
	return (0);
}

int	is_empty(char **arry)
{
	int	i;

	i = 0;
	while (arry[i])
		i++;
	if (i < 6)
		return (1);
	return (0);
}

int	is_color_dup(t_file_data *file_data)
{
	int	i;
	int	check;

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

void	fill_map(char *line, t_file_data *file_data, int *update_map_arr)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	file_data->map[*update_map_arr] = malloc(ft_strlen(line));
	while (line[i])
	{
		file_data->map[*update_map_arr][i] = line[i];
		i++;
	}
	check = 1;
	if (check)
		(*update_map_arr)++;
}

int	set_data(int fd, t_file_data *file_data)
{
	char		*buffer;
	char		**to_be_splited;
	t_cmp_data	*cmp_data;
	int			i;
	char		**already_checked;
	int			value;
	int			*update_map_arr;
	int			fill_only_map;

	cmp_data = malloc(sizeof(t_cmp_data));
	reset_data(cmp_data);
	i = 0;
	value = 0;
	update_map_arr = &value;
	fill_only_map = 0;
	already_checked = ft_calloc(7, sizeof(char *));
	file_data->map = malloc(file_data->map_size * sizeof(char *));
	while (1)
	{
		buffer = get_next_line(fd);
		if (buffer)
		{
			to_be_splited = ft_split(buffer, " ");
			if (!match_in_list(to_be_splited[0], cmp_data->compass,
					already_checked))
			{
				if (store_in_the_right_place(to_be_splited, file_data))
					return (1);
			}
			else if (match_in_list(to_be_splited[0], cmp_data->compass,
					already_checked) == 2)
				return (1);
			if (!is_empty(already_checked) && fill_only_map)
				fill_map(buffer, file_data, update_map_arr);
			if (!is_empty(already_checked))
				fill_only_map = 1;
		}
		else
			break ;
	}
	if (is_empty(already_checked) || is_color_dup(file_data))
		return (1);
	return (0);
}
