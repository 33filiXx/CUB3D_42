/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:32:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 20:32:35 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

unsigned int texel(t_texture *tex, int x, int y)
{
    char    *addr;

    if (!tex || !tex->addr || x < 0 || y < 0
        || x >= tex->width || y >= tex->height)
        return (0);
    addr = tex->addr + y * tex->line_len + x * (tex->bpp / 8);
    return (*(unsigned int *)addr);
}

bool texture_is_transparent(t_texture *tex, unsigned int color)
{
    unsigned int    masked_color;

    masked_color = color & 0x00FFFFFF;
    if (!tex || !tex->addr)
        return (masked_color == 0);
    if (tex->has_transparency)
    {
        unsigned int masked_ref;

        masked_ref = tex->transparent_color & 0x00FFFFFF;
        if (masked_color == masked_ref)
            return (true);
    }
    return (masked_color == 0);
}
void    set_frame(t_sprite *sprite)
{
    sprite->frame_w = SPRITE_FRAME_SIZE;
    sprite->frame_h = SPRITE_FRAME_SIZE;
    sprite->frame_cols = sprite->sp_tex->width / sprite->frame_w;
    if (sprite->frame_cols <= 0)
        sprite->frame_cols = 1;
    sprite->frame_rows = sprite->sp_tex->height / sprite->frame_h;
    if (sprite->frame_rows <= 0)
        sprite->frame_rows = 1;
    sprite->frame_count = sprite->frame_cols * sprite->frame_rows;
    if (sprite->frame_count <= 0)
        sprite->frame_count = 1;
}

void    sprite_sheet_init(t_game_data *data, t_sprite *sprite)
{
    static t_texture sheet;
    static bool     loaded;

    if (!sprite)
        return ;
    if (!loaded)
    {
        sheet.mlx_connection = data->mlx.mlx_connection;
        load_texture(data, &sheet, "../../textures/spritesheet.xpm");
        if (sheet.width > 0 && sheet.height > 0)
        {
            sheet.transparent_color = texel(&sheet, 0, 0);
            sheet.has_transparency = true;
        }
        loaded = true;
    }
    sprite->sp_tex = &sheet;
    set_frame(sprite);
}

void    append_to_sprite(t_game_data *data, int x , int y)
{
    t_sprite *sprite;
    t_sprite *tmp;
    size_t new_count;

    new_count = data->sprite_count + 1;
    tmp = realloc(data->sprite, sizeof(t_sprite) * new_count);
    if(!tmp)
        return ;
    data->sprite = tmp;
    sprite = &data->sprite[new_count - 1];
    ft_bzero(sprite, sizeof(*sprite));
    sprite->map_x = x;
    sprite->map_y = y;
    sprite->position = vec2_new(x + 0.5, y + 0.5);
    sprite->frame = 0;
    sprite->frame_time = 0;
    sprite->frame_duration = 0.12;
    sprite->speed = 0.4;
    sprite->velocity = vec2_new(0, 0);
    sprite->state = SPRITE_IDLE;
    sprite->visible = false;
    sprite_sheet_init(data, sprite);
    data->sprite_count = new_count;
}