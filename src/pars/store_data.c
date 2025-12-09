/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 17:05:41 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 01:58:43 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void fill_map(char *line, t_file_data *file_data, int *update_map_arr)
{
    int i;

    i = 0;
    if (skip_empty_line(line) && *update_map_arr == 0)
        return;
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

int already_checked_lenght(char **already_checked)
{
    int i;

    i = 0;
    while (already_checked[i])
        i++;
    return i;
}

int set_data_helper(t_file_data *file_data, t_cmp_data *cmp_data, t_helper_data *helper_data)
{
    helper_data->to_be_splited = ft_split(helper_data->buffer, ' ');
    if (!(helper_data->fill_only_map))
    {
        if (!match_in_list(helper_data->to_be_splited[0], cmp_data->compass,
                           helper_data->already_checked))
        {
            if (store_in_the_right_place(helper_data->to_be_splited, file_data, helper_data->buffer))
            {
                cleanup_inside_set_data(cmp_data, helper_data->already_checked, helper_data->to_be_splited, helper_data->buffer);
                return (free(helper_data), 1);
            }
        }
        else if (match_in_list(helper_data->to_be_splited[0], cmp_data->compass,
                               helper_data->already_checked) != 0)
        {
            cleanup_inside_set_data(cmp_data, helper_data->already_checked, helper_data->to_be_splited, helper_data->buffer);
            return (free(helper_data), 1);
        }
    }
    if (helper_data->buffer && !is_empty(helper_data->already_checked) && helper_data->fill_only_map)
        fill_map(helper_data->buffer, file_data, helper_data->update_map_arr);
    if (!is_empty(helper_data->already_checked))
        helper_data->fill_only_map = 1;
    free_double_array(helper_data->to_be_splited);
    return (free(helper_data->buffer), 0);
}

int set_clean_helep(t_file_data *file_data, t_cmp_data *cmp_data, t_helper_data *helper_data)
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

int set_data(int fd, t_file_data *file_data)
{
    t_cmp_data *cmp_data;
    t_helper_data *helper_data;

    helper_data = malloc(sizeof(t_helper_data));
    cmp_data = malloc(sizeof(t_cmp_data));
    reset_data_helper(helper_data, 2);
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
            break;
    }
    file_data->map[helper_data->value] = NULL;
    if (set_clean_helep(file_data, cmp_data, helper_data))
        return (1);
    return (0);
}
