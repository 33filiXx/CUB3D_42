/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 02:06:50 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/23 16:51:25 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	last_floor(char *line, int j)
{
	int	i;

	i = j + 1;
	while (line[i] && line[i] != '+' && line[i] != '\n')
	{
		if (line[i] == '0')
			return (1);
		i++;
	}
	return (0);
}

int	is_door_valid(char *str, char *next, char *prev)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 'D')
		{
			if ((str[i - 1] == '1' && str[i + 1] == '1')
				|| (next[i] == '1' && prev[i] == '1'))
				return (0);
			else
				return (1);
		}
		i++;
	}
	return (0);
}

int	is_valid_helper_o(t_file_data *file_data, t_helper_data *helper_data)
{
	if (file_data->map[helper_data->i][helper_data->j + 1] == '+'
				|| file_data->map[helper_data->i][helper_data->j + 1] == '\n')
		return (1);
	if (file_data->map[helper_data->i - 1][helper_data->j] == '+'
			|| file_data->map[helper_data->i + 1][helper_data->j] == '+'
				|| file_data->map[helper_data->i - 1][helper_data->j] == ' '
				||file_data->map[helper_data->i + 1][helper_data->j] == ' ')
		return (1);
	return (0);
}

int	is_valid_helper(t_file_data *file_data, t_helper_data *helper_data)
{
	if (file_data->map[0][helper_data->j] != '1'
		&& file_data->map[0][helper_data->j] != '+'
			&& file_data->map[0][helper_data->j] != ' ')
		return (1);
	if (file_data->map[helper_data->i][helper_data->start] != '1')
		return (1);
	if (helper_data->i >= 1 && is_door_valid(file_data->map[helper_data->i],
			file_data->map[helper_data->i + 1],
			file_data->map[helper_data->i - 1]))
		return (1);
	if (file_data->map[helper_data->i + 1] && helper_data->i >= 1
		&& file_data->map[helper_data->i][helper_data->j] == '0')
	{
		if (is_valid_helper_o(file_data, helper_data))
			return (1);
	}
	if (!file_data->map[helper_data->i + 1])
		if (file_data->map[helper_data->i][helper_data->j] != '1'
				&& file_data->map[helper_data->i][helper_data->j] != '+')
			return (1);
	return (0);
}

int	set_data(int fd, t_file_data *file_data)
{
	t_cmp_data		*cmp_data;
	t_helper_data	*helper_data;

	helper_data = malloc(sizeof(t_helper_data));
	cmp_data = malloc(sizeof(t_cmp_data));
	reset_d_h(helper_data, 2);
	helper_data->update_map_arr = &helper_data->value;
	reset_data(cmp_data);
	file_data->map = ft_calloc(file_data->map_size + 1, sizeof(char *));
	while (1)
	{
		helper_data->buffer = get_next_line(fd);
		if (helper_data->buffer)
		{
			if (set_data_helper(file_data, cmp_data, helper_data))
				return (1);
		}
		else
			break ;
	}
	file_data->map[helper_data->value] = NULL;
	if (set_clean_helep(file_data, cmp_data, helper_data))
		return (1);
	return (0);
}
