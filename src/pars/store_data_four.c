/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data_four.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 01:56:05 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 02:25:32 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	check_if_exact(char **str)
{
	if (str[2])
		return (1);
	else
		return (0);
}

int	store_texture(char **split, char **dst)
{
	if (check_if_exact(split))
		return (1);
	*dst = dup_trimmed_token(split[1]);
	if (!*dst)
		return (1);
	return (0);
}

int	store_color(t_file_data *file_data, char *buffer, char type)
{
	if (!comma_length_checker(buffer))
	{
		if (specific_store(file_data, type, ft_strchr(buffer, type)))
			return (1);
	}
	else
		return (1);
	return (0);
}

int	store_in_the_right_place(char **to_be_splited, t_file_data *file_data, char *buffer)
{
	if (!strcmp(to_be_splited[0], NO))
		return (store_texture(to_be_splited, &file_data->no_texture));
	else if (!strcmp(to_be_splited[0], SO))
		return (store_texture(to_be_splited, &file_data->so_texture));
	else if (!strcmp(to_be_splited[0], WE))
		return (store_texture(to_be_splited, &file_data->we_texture));
	else if (!strcmp(to_be_splited[0], EA))
		return (store_texture(to_be_splited, &file_data->ea_texture));
	else if (!strcmp(to_be_splited[0], F))
		return (store_color(file_data, buffer, 'F'));
	else if (!strcmp(to_be_splited[0], C))
		return (store_color(file_data, buffer, 'C'));
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
