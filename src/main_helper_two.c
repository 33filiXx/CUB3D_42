/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:26:18 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 17:27:31 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	parse_and_merge(t_game_data *game_data, char **av)
{
	t_file_data	*file_data;

	file_data = ft_calloc(1, sizeof(t_file_data));
	if (!file_data)
		return (report_error("Allocation error"));
	if (punisher(av + 1, file_data))
	{
		free_file_data(file_data);
		return (report_error("Parsing failed"));
	}
	if (merge_data(game_data, file_data))
	{
		free_file_data(file_data);
		return (report_error("Failed to merge parsed data"));
	}
	free(file_data);
	return (0);
}

int	report_error(char *message)
{
	ft_putendl_fd(message, 2);
	return (1);
}
