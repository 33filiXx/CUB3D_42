/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:19:29 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/10/28 09:50:14 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

#include <stdio.h>

void	print_file_data(t_file_data data)
{
	int	i;

	printf("NO Texture: %s\n", data.no_texture);
	printf("SO Texture: %s\n", data.so_texture);
	printf("WE Texture: %s\n", data.we_texture);
	printf("EA Texture: %s\n", data.ea_texture);
	printf("maps_size: %d\n", data.s_element_size);
	printf("player postion row: %d\n", data.row);
	printf("column: %d\n", data.column);

	printf("Floor Color:   [%d, %d, %d]\n",
		data.floor_color[0], data.floor_color[1], data.floor_color[2]);

	printf("Ceiling Color: [%d, %d, %d]\n",
		data.ceiling_color[0], data.ceiling_color[1], data.ceiling_color[2]);

	// Print map
	if (data.map)
	{
		printf("\nMap:	\n");
		i = 0;
		while (data.map[i])
		{
			printf("%s", data.map[i]);
			i++;
		}
	}
	else
		printf("Map: (null)\n");
}

int main(int ac , char **av)
{

    t_file_data *file_data;

    file_data = malloc(sizeof(t_file_data));
    if (ac == 2)
    {
        if(!punisher(av + 1 , file_data))
        {
            print_file_data(*file_data);
            exit(22);
        }
        else
        {
            printf("error\n");
            exit(55);
        }
    }
}