/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:19:29 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/30 16:47:25 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"


int main(int ac , char **av)
{

    t_file_data *file_data;

    file_data = malloc(sizeof(t_file_data));
    if (ac == 2)
    {
        if(punisher(av + 1 , file_data))
            exit(22);
        else
            exit(55);
    }
}