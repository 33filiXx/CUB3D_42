/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:30:55 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 20:31:23 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int get_visible_s_len(t_game_data *data)
{
    int i;
    int len;

    i = 0;
    len = 0;
    while(i < data->sprite_count)
    {
        if(data->sprite[i].visible == true)
            len++;
        i++;
    }
    return (len);
}

int    collect_visible_sprites(t_game_data *data, t_sprite ***visible_sprites)
{
    int i;
    int len;
    int incr;

    len = get_visible_s_len(data);
    i = 0;
    incr = 0;
    if (!len)
    {
        *visible_sprites = NULL;
        return (0);
    }
    *visible_sprites = malloc(sizeof(t_sprite *) * len);
    if (!*visible_sprites)
        return (0);
    while(i < data->sprite_count)
    {
        if(data->sprite[i].visible == true)
        {
            (*visible_sprites)[incr] = &data->sprite[i];
            incr++;
        }
        i++;
    }
    return (len);
}

void    swap(t_sprite **v_sprites, int i, int j)
{
    t_sprite *tmp;

    tmp = v_sprites[i];
    v_sprites[i] = v_sprites[j];
    v_sprites[j] = tmp;
}

void    free_sprites(t_sprite **visible_sprites , int len)
{
    (void)len;
    free(visible_sprites);
}

t_sprite    **sort_sprites(t_game_data *data)
{
    int i = 0;
    int j = 0;
    t_sprite **visible_sprites;
    
    data->v_sprite_counter = collect_visible_sprites(data, &visible_sprites);
    if (!data->v_sprite_counter || !visible_sprites)
        return (NULL);
    while (i < data->v_sprite_counter - 1)
    {
        j = i + 1;
        while (j < data->v_sprite_counter)
        {
            if (visible_sprites[i]->cam_z < visible_sprites[j]->cam_z)
                swap(visible_sprites, i, j);
            j++;
        }
        i++;
    }
    return (visible_sprites);
}