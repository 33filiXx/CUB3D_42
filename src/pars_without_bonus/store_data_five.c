/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data_five.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 01:57:32 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 02:43:23 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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

int	is_only_space(char *str)
{
	int	i;
	int	just_space;

	i = 0;
	just_space = 1;
	if (!str)
	{
		return (1);
	}
	while (str[i])
	{
		if (str[i] != ' ' || str[i] != '\n')
		{
			just_space = 0;
		}
		i++;
	}
	return (just_space);
}

int	is_betwen(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

void	update_line(char *line)
{
	int	i;
	int	check;
	int	is_bet;

	i = 0;
	check = 0;
	is_bet = 0;
	while (line[i])
	{
		if ((line[i] != ' ' && line[i] != '\t'))
			check = 1;
		if (check && (line[i] == ' ' || line[i] == '\t'))
		{
			if (is_betwen(&line[i]))
				line[i] = '1';
		}
		else if (!check && (line[i] == ' ' || line[i] == '\t'))
			line[i] = '+';
		i++;
	}
}

int	skip_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ' || line[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
