/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:08:34 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 20:32:44 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_frame    get_frame_within_sheet(t_sprite *sprite)
{
    t_frame frame;

    frame.cols = (sprite->frame_cols > 0) ? sprite->frame_cols : 1;
    frame.rows = (sprite->frame_rows > 0) ? sprite->frame_rows : 1;
    sprite->draw.frame_col = sprite->frame % frame.cols;
    sprite->draw.frame_row = sprite->frame / frame.cols;
    if (sprite->draw.frame_row >= frame.rows)
        sprite->draw.frame_row = frame.rows - 1;
    sprite->draw.origin_x = sprite->draw.frame_col * sprite->frame_w;
    sprite->draw.origin_y = sprite->draw.frame_row * sprite->frame_h;
    frame.span_x = sprite->draw.draw_end_x - sprite->draw.draw_start_x;
    if (frame.span_x < 1)
        frame.span_x = 1;
    frame.span_y = sprite->draw.draw_end_y - sprite->draw.draw_start_y;
    if (frame.span_y < 1)
        frame.span_y = 1;
    return (frame);
}

void    render_helper(t_sprite *sprite, t_frame *frame, t_game_data *data, t_render_sprite *render_s)
{
    while (render_s->y <= sprite->draw.draw_end_y)
    {
        render_s->v = (double)(render_s->y - sprite->draw.draw_start_y) / (double)frame->span_y;
        if (render_s->v < 0.0)
            render_s->v = 0.0;
        if (render_s->v > 1.0)
            render_s->v = 1.0;
        render_s->tex_y = sprite->draw.origin_y + (int)(render_s->v * sprite->frame_h);
        if (render_s->tex_y >= sprite->draw.origin_y + sprite->frame_h)
            render_s->tex_y = sprite->draw.origin_y + sprite->frame_h - 1;
        if (render_s->tex_y >= sprite->sp_tex->height)
            render_s->tex_y = sprite->sp_tex->height - 1;
        render_s->color = texel(sprite->sp_tex, render_s->tex_x, render_s->tex_y);
        if (!texture_is_transparent(sprite->sp_tex, render_s->color))
            put_pixel(&data->mlx, render_s->x , render_s->y, render_s->color);
        render_s->y++;
    }
}

void    render_sprites(t_sprite *sprite, t_frame *frame, t_game_data *data)
{
    t_render_sprite render_s;

    render_s.x = sprite->draw.draw_start_x;
    while (render_s.x <= sprite->draw.draw_end_x)
    {
        if (!data->z_buffer || render_s.x < 0 || render_s.x >= data->z_buffer_size
            || data->z_buffer[render_s.x] <= 0.0 || sprite->cam_z >= data->z_buffer[render_s.x])
        {
            render_s.x++;
            continue;
        }
        render_s.u = (double)(render_s.x - sprite->draw.draw_start_x) / (double)frame->span_x;
        if (render_s.u < 0.0)
            render_s.u = 0.0;
        if (render_s.u > 1.0)
            render_s.u = 1.0;
        render_s.tex_x = sprite->draw.origin_x + (int)(render_s.u * sprite->frame_w);
        if (render_s.tex_x >= sprite->draw.origin_x + sprite->frame_w)
            render_s.tex_x = sprite->draw.origin_x + sprite->frame_w - 1;
        if (render_s.tex_x >= sprite->sp_tex->width)
            render_s.tex_x = sprite->sp_tex->width - 1;
        render_s.y = sprite->draw.draw_start_y;
        render_helper(sprite, frame, data, &render_s);
        render_s.x++;
    }
}

void    sprite_draw(t_game_data *data, t_sprite *sprite, int start_x, int v_w, int v_h)
{
    t_frame frame;

    if (!sprite || sprite->cam_z <= 0.1 || !sprite->sp_tex || !sprite->sp_tex->addr)
        return ;
    sprite->draw.inv_z = 1.0 / sprite->cam_z;
    project_to_screen(sprite, start_x, v_w, v_h);
    if (sprite->draw.sprite_height <= 0 || sprite->draw.sprite_width <= 0)
        return ;
    clamp_drawing_bounds(sprite, start_x, v_w, v_h);
    if (sprite->draw.draw_start_x > sprite->draw.draw_end_x)
        return ;
    frame = get_frame_within_sheet(sprite);
    render_sprites(sprite, &frame, data);
}

void sprite_render_all(t_game_data *data, int start_x, int view_w, int view_h)
{
    t_sprite **visible;
    int i;

    if (!data->sprite || data->sprite_count <= 0)
        return ;
    visible = sort_sprites(data);
    i = 0;
    if (!visible)
        return ;
    while (i < data->v_sprite_counter)
    {
        sprite_draw(data, visible[i], start_x, view_w, view_h);
        i++;
    }
    free(visible);
}

