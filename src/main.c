/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:19:29 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/11/27 09:24:37 by wel-mjiy         ###   ########.fr       */
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




void    free_cmp_data(t_cmp_data *cmp)
{
    int i;

    if (!cmp)
        return;

    if (cmp->compass)
    {
        i = 0;
        while (cmp->compass[i])
        {
            free(cmp->compass[i]);
            i++;
        }
        free(cmp->compass);
    }

    free(cmp);
}
void    free_double_array(char **arr)
{
    int i;

    if (!arr)
        return;

    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}


void    free_file_data(t_file_data *data)
{
    int i;

    if (!data)
        return;

    free(data->no_texture);
    free(data->so_texture);
    free(data->we_texture);
    free(data->ea_texture);

    if (data->map)
    {
        i = 0;
        while (i < data->map_size)
        {
            free(data->map[i]);
            i++;
        }
        free(data->map);
    }
    free(data);
}




int main(int ac , char **av)
{

    t_file_data *file_data;

    file_data = ft_calloc(1, sizeof(t_file_data));
    if (ac == 2)
    {
        if(!punisher(av + 1 , file_data))
        {
            print_file_data(*file_data);
        }
        else
        {
            free_file_data(file_data);
            printf("error\n");
            exit(55);
        }
    }
    free_file_data(file_data);
	
}