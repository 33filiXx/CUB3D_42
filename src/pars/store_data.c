/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 17:05:41 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/23 17:22:16 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	fill_map(char *line, t_file_data *file_data, int *update_map_arr)
{
	int	i;

	i = 0;
	if (skip_empty_line(line) && *update_map_arr > file_data->map_size - 5)
		return ;
	if (skip_empty_line(line) && *update_map_arr == 0)
		return ;
	file_data->map[*update_map_arr] = malloc(file_data->element_size);
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

int	already_checked_lenght(char **already_checked)
{
	int	i;

	i = 0;
	while (already_checked[i])
		i++;
	return (i);
}

int	set_data_helper_h(t_file_data *file_data, t_cmp_data *cmp_data,
			t_helper_data *helper_data)
{
	if (!match_in_list(helper_data->to_be_splited[0],
			cmp_data->compass, helper_data->already_checked))
	{
		if (store_r_p(helper_data->to_be_splited, file_data,
				helper_data->buffer))
		{
			cleanup_i_d(cmp_data, helper_data->already_checked,
				helper_data->to_be_splited, helper_data->buffer);
			return (free(helper_data), 1);
		}
	}
	else if (match_in_list(helper_data->to_be_splited[0],
			cmp_data->compass, helper_data->already_checked) != 0)
	{
		cleanup_i_d(cmp_data, helper_data->already_checked,
			helper_data->to_be_splited, helper_data->buffer);
		return (free(helper_data), 1);
	}
	return (0);
}

int em(char *line)
{
	int i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ')
			return 1;
		i++;
	}
	return 0;
}

int	set_data_helper(t_file_data *file_data, t_cmp_data *cmp_data,
			t_helper_data *helper_data)
{
	helper_data->to_be_splited = ft_split(helper_data->buffer, ' ');
	if (!(helper_data->fill_only_map))
	{
		if (set_data_helper_h(file_data, cmp_data, helper_data))
			return (1);
	}
	if (helper_data->buffer && !is_empty(helper_data->already_checked)
		&& helper_data->fill_only_map)
		fill_map(helper_data->buffer, file_data, helper_data->update_map_arr);
	if (!is_empty(helper_data->already_checked))
		helper_data->fill_only_map = 1;
	free_double_array(helper_data->to_be_splited);
	return (free(helper_data->buffer), 0);
}

int	set_clean_helep(t_file_data *file_data,
		t_cmp_data *cmp_data, t_helper_data *helper_data)
{
	if (is_empty(helper_data->already_checked))
	{
		free_cmp_data(cmp_data);
		free_double_array(helper_data->already_checked);
		free(helper_data);
		return (1);
	}
	file_data->s_element_size++;
	free_cmp_data(cmp_data);
	free_double_array(helper_data->already_checked);
	free(helper_data);
	return (0);
}
