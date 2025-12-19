/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:18:41 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 02:19:54 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	get_size(int fd, t_file_data *file_data)
{
	int		map_length;
	char	*buffer;
	int		element_length;

	element_length = 0;
	map_length = 0;
	while (1)
	{
		buffer = get_next_line(fd);
		if (buffer && ft_strlen(buffer) > element_length)
			element_length = ft_strlen(buffer);
		map_length++;
		if (!buffer)
			break ;
		if (buffer)
			free(buffer);
	}
	file_data->map_size = map_length;
	file_data->element_size = element_length + 1;
}

int	punisher(char **av, t_file_data *file_data)
{
	int	fd;
	int	new_fd;

	fd = exists(*av);
	if (fd == -1)
		return (1);
	if (valid_extantion(*av))
		return (1);
	get_size(fd, file_data);
	file_data->s_element_size = 0;
	close(fd);
	new_fd = open(*av, O_RDONLY);
	if (new_fd == -1)
		return (1);
	if (storing(new_fd, file_data))
		return (1);
	return (0);
}
