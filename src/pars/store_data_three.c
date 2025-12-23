/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data_three.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 01:54:30 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/20 20:03:27 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	handle_comma(char **tmp, t_helper_data *h, int *arr)
{
	(*tmp)[h->p] = '\0';
	h->check = 0;
	if (ft_atoi(*tmp) > 255)
	{
		free(*tmp);
		return (1);
	}
	arr[(h->j)++] = ft_atoi(*tmp);
	free(*tmp);
	*tmp = malloc(4);
	h->p = 0;
	return (0);
}

int	parse_step(char *buf, char **tmp, t_helper_data *h, int *arr)
{
	if (buf[h->i] >= '0' && buf[h->i] <= '9')
	{
		(*tmp)[(h->p)++] = buf[h->i];
		h->check = 1;
	}
	else if (buf[h->i] == ',')
		return (handle_comma(tmp, h, arr));
	else if (buf[h->i] != ' ')
	{
		free(*tmp);
		return (1);
	}
	if (h->check && buf[h->i] == ' ' && !next_one(buf + h->i))
	{
		free(*tmp);
		return (1);
	}
	return (0);
}

int	finalize_store(char *tmp, t_helper_data *h, int *arr)
{
	if (h->check && h->p > 0)
	{
		tmp[h->p] = '\0';
		if (ft_atoi(tmp) > 255)
		{
			free(tmp);
			return (1);
		}
		if (h->j < 3)
			arr[(h->j)++] = ft_atoi(tmp);
	}
	free(tmp);
	return (0);
}

int	*get_target_array(t_file_data *file_data, char who_know)
{
	if (who_know == 'F')
		return (file_data->floor_color);
	return (file_data->ceiling_color);
}

int	specific_store(t_file_data *file_data, char who_know, char *buffer)
{
	char			*tmp;
	t_helper_data	*h;
	int				*arr;
	int				result;

	h = malloc(sizeof(t_helper_data));
	if (!h)
		return (1);
	reset_d_h(h, 1);
	tmp = malloc(4);
	arr = get_target_array(file_data, who_know);
	while (buffer[h->i] && buffer[h->i] != '\n')
	{
		if (parse_step(buffer, &tmp, h, arr))
		{
			free(h);
			return (1);
		}
		h->i++;
	}
	result = finalize_store(tmp, h, arr);
	free(h);
	return (result);
}
