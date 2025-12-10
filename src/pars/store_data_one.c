/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data_one.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 01:49:00 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 04:37:43 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

char	*dup_trimmed_token(char *token)
{
	char	*trimmed;
	char	*result;

	if (!token)
		return (NULL);
	trimmed = ft_strtrim (token, " \t\n\r");
	if (!trimmed)
		return (NULL);
	result = ft_strdup(trimmed);
	free(trimmed);
	return (result);
}

void	cleanup_i_d(t_cmp_data	*cmp_data,
	char **already_checked, char **to_be_splited, char *buffer)
{
	if (cmp_data)
		free_cmp_data(cmp_data);
	if (already_checked)
		free_double_array(already_checked);
	if (to_be_splited)
		free_double_array(to_be_splited);
	free(buffer);
}

void	reset_data(t_cmp_data *cmp_data)
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

void	reset_d_h(t_helper_data *helper_data, int flag)
{
	helper_data->i = 0;
	if (flag)
		helper_data->i = 1;
	helper_data->j = 0;
	helper_data->p = 0;
	helper_data->value = 0;
	helper_data->check = 0;
	helper_data->checked = 0;
	if (flag == 2)
		helper_data->already_checked = ft_calloc(7, sizeof(char *));
	helper_data->buffer = NULL;
	helper_data->to_be_splited = NULL;
	helper_data->update_map_arr = NULL;
	helper_data->fill_only_map = 0;
	helper_data->start = 0;
}

int	is_deff_line(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '\t' && str[i] != '\n' && str[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}
