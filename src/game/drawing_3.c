/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 19:01:13 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/01 19:19:55 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	*set_ints(int i, int j, int color)
{
	int	*int_holder;

	int_holder = malloc(sizeof(int ) * 3);
	int_holder[0] = i;
	int_holder[1] = j;
	int_holder[2] = color;
	return (int_holder);
}

int	get_ty(t_cercle *cercle, t_game_data *data, t_minimap *minimap)
{
	set_cercle_data(data, minimap, cercle);
	return (cercle->c_y - cercle->r);
}
