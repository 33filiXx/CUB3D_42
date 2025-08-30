/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:34:54 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/30 17:26:51 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"


int set_data(int fd , t_file_data *file_data)
{
    char *buffer;
    char **to_be_splited;
	int i;

	i = 0;

    while (1)
    {
        buffer = get_next_line(fd);
        if(buffer)
		{
            to_be_splited = ft_split(buffer ,"  , ");
			while (to_be_splited[i])
			{
				
			}
			
		}
		
    }
    
}