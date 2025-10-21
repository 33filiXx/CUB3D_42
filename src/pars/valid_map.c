/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:34:51 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/10/17 01:23:14 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"



void reset_map_info(char *map_info)
{
	map_info = malloc(8 * sizeof(char));
	map_info[0] = '0';
	map_info[1] = '1';
	map_info[2] = 'W';
	map_info[3] = 'N';
	map_info[4] = 'S';
	map_info[5] = 'E';
	map_info[6] = '\0';
}

int is_valid(int fd, t_file_data *file_data)
{
    // char		*map_info;
	// reset_map_info(map_info);
	// int	spawning;
    
	// spawning = 0;
    if(set_data(fd , file_data))
        return 1;
    return 0;
}