/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 01:51:35 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/23 17:26:58 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	match_in_list_h(char *s1, char **already_checked,
	t_helper_data *helper_data)
{
	if (!already_checked[helper_data->checked])
	{
		already_checked[helper_data->checked] = ft_strdup(s1);
		return (0);
	}
	while (already_checked[helper_data->checked])
	{
		if (!ft_strcmp(s1, already_checked[helper_data->checked]))
		{
			return (2);
		}
		(helper_data->checked)++;
	}
	already_checked[helper_data->checked] = ft_strdup(s1);
	return (0);
}

int	match_in_list(char *s1, char **s2, char **already_checked)
{
	t_helper_data	*helper_data;

	if (!s1 || !s2)
		return (1);
	helper_data = malloc(sizeof(t_helper_data));
	reset_d_h(helper_data, 0);
	while (s2[helper_data->j])
	{
		helper_data->i = 0;
		while (s1 && s2[helper_data->j] && s1[helper_data->i]
			&& s2[helper_data->j][helper_data->i]
				&& s1[helper_data->i] == s2[helper_data->j][helper_data->i])
			(helper_data->i)++;
		if (!(s1[helper_data->i] - s2[helper_data->j][helper_data->i]))
		{
			if (match_in_list_h(s1, already_checked, helper_data) == 2)
				return (free(helper_data), 2);
			else
				return (free(helper_data), 0);
		}
		(helper_data->j)++;
	}
	if (is_deff_line(s1))
		return (free(helper_data), 1);
	return (free(helper_data), 0);
}

int	comma_length_checker_helper(char *str, int *i, int *counter, int *comma)
{
	while (str[*i])
	{
		if (str[*i] >= '0' && str[*i] <= '9')
		{
			(*counter)++;
			if (*counter > 3)
				return (1);
		}
		else if (str[*i] == ',')
		{
			*counter = 0;
			(*comma)++;
		}
		if (*comma > 2)
			return (1);
		(*i)++;
	}
	return (0);
}

int	comma_length_checker(char *str)
{
	int (i), (counter), (comma);
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
	if (comma_length_checker_helper(str, &i, &counter, &comma) == 1)
		return (1);
	return (0);
}

int	next_one(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ','|| str[i] == '\n')
			return (1);
		else if ((str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (0);
}
