/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:08:34 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/21 22:51:30 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define SPRITE_FRAME_SIZE 52
#define SPRITE_FRAME_COUNT 25

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
        loaded = true;
    }
    sprite->sp_tex = &sheet;
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

void sprite_load_map(t_game_data *data)
{
    int y = 0;
    int x = 0;
     
    // free_previous_sprites(data); // when levels are added we ll need this one
    if (data->sprite)
        free(data->sprite);
    data->sprite = NULL;
    data->sprite_count = 0;
    while (y < data->map.height)
    {
        x = 0;
        while (x < data->map.width)
        {
            if (data->map.grid[y][x] == 'S')
                append_to_sprite(data, x, y);
            x++;
        }
        y++;
    }
}

void    sprite_animate(t_sprite *sprite, double dt)
{
    if (!sprite || sprite->frame_count <= 0)
        return ;
    sprite->frame_time += dt;
    if (sprite->frame_time >= sprite->frame_duration)
    {
        sprite->frame_time -= sprite->frame_duration;
        sprite->frame = (sprite->frame + 1) % sprite->frame_count;
    }
}

void    sprite_try_move(t_game_data *data, t_sprite *sprite, t_vec2 step)
{
    t_vec2 test_x;
    t_vec2 test_y;

    test_x.x = step.x;
    test_x.y = 0;
    test_y.x = 0;
    test_y.y = step.y;    
    test_x = vec2_add(sprite->position, test_x);
    if (valid_move(data, test_x.x, test_x.y))
    {
        sprite->position = test_x;
        sprite->map_x = (int)floor(sprite->position.x);
        sprite->map_y = (int)floor(sprite->position.y);
        return ;
    }
    test_y = vec2_add(sprite->position, test_y);
    if (valid_move(data, test_y.x, test_y.y))
    {
        sprite->position = test_y;
        sprite->map_x = (int)floor(sprite->position.x);
        sprite->map_y = (int)floor(sprite->position.y);
        return ;
    }
}

void    sprite_update_one(t_game_data *data, t_sprite *sprite, double dt)
{
    t_vec2 to_player;
    t_vec2  dir;
    t_vec2  step;
    double len2;
    
    sprite_animate(sprite, dt);
    to_player = vec2_sub(data->player.pos, sprite->position);
    len2 = vec2_dot(to_player , to_player);  //to_player vec dist is : sqrt(len2)
    if (len2 > 1e-6)
    {
        dir = vec2_scale(to_player, 1.0 / sqrt(len2));
        step = vec2_scale(dir, sprite->speed * dt);
        sprite_try_move(data, sprite, step);
    }
}

int    collect_visible_sprites(t_game_data *data, t_sprite ***visible_sprites)
{
    int i = 0;
    int len = 0;
    int incr = 0;

    while(i < data->sprite_count)
    {
        if(data->sprite[i].visible == true)
            len++;
        i++;
    }
    i = 0;
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

void    sprite_camera_transform(t_game_data *data, t_sprite *sprite)
{
    t_vec2  rel;
    double  det;
    double  inv_det;
    
    rel = vec2_sub(sprite->position, data->player.pos);
    det = data->player.plane.x * data->player.dir.y
        - data->player.dir.x * data->player.plane.y;
    if (fabs(det) < 1e-9)
    {
        sprite->visible = false;
        return ;
    }
    inv_det = 1.0 / det;
    sprite->cam_x = inv_det * (data->player.dir.y * rel.x
            - data->player.dir.x * rel.y);
    sprite->cam_z = inv_det * (-data->player.plane.y * rel.x
            + data->player.plane.x * rel.y);
    if (sprite->cam_z > 0.1)
    {
        sprite->dist_cam = sqrt(vec2_dot(rel, rel));
        sprite->visible = true;
    }
    else
    {
        sprite->dist_cam = 0.0;
        sprite->visible = false;
    }
}

void sprite_update_all(t_game_data *data, double dt)
{
    int i;

    if (!data->sprite || data->sprite_count <= 0)
        return ;
    i = 0;
    while (i < data->sprite_count)
    {
        sprite_update_one(data, &data->sprite[i], dt);
        sprite_camera_transform(data, &data->sprite[i]);
        i++;
    }
}
unsigned int texel(t_texture *tex, int x, int y)
{
    char *addr;

    if (!tex || !tex->addr)
        return (0);
    addr = tex->addr + y * tex->line_len + x * (tex->bpp / 8);
    return *(unsigned int *)addr;
}
bool is_transparent(unsigned int color)
{
    return ((color & 0xFF000000U) == 0);
}
void    sprite_draw(t_game_data *data, t_sprite *sprite, int start_x, int v_w, int v_h)
{
    int span_x;
    int span_y;
    int cols;
    int rows;
    int x;
    int y;
    int tex_x;
    int tex_y;
    double u;
    double v;
    unsigned int color;

    if (!sprite || sprite->cam_z <= 0.1 || !sprite->sp_tex || !sprite->sp_tex->addr)
        return ;
    sprite->draw.inv_z = 1.0 / sprite->cam_z;
    // project to screen
    sprite->draw.screen_x = start_x + (int)((v_w / 2.0)
            * (1.0 + sprite->cam_x * sprite->draw.inv_z));
    sprite->draw.sprite_height = (int)fabs(v_h * sprite->draw.inv_z);
    sprite->draw.sprite_width = sprite->draw.sprite_height;
    if (sprite->draw.sprite_height <= 0 || sprite->draw.sprite_width <= 0)
        return ;
    // clamp_drawing_bounds
    sprite->draw.draw_start_y = -sprite->draw.sprite_height / 2 + v_h / 2;
    sprite->draw.draw_end_y = sprite->draw.sprite_height / 2 + v_h / 2;
    if (sprite->draw.draw_start_y < 0)
        sprite->draw.draw_start_y = 0;
    if (sprite->draw.draw_end_y >= v_h)
        sprite->draw.draw_end_y = v_h - 1;
    sprite->draw.draw_start_x = -sprite->draw.sprite_width / 2 + sprite->draw.screen_x;
    sprite->draw.draw_end_x = sprite->draw.sprite_width / 2 + sprite->draw.screen_x;
    if (sprite->draw.draw_start_x < start_x)
        sprite->draw.draw_start_x = start_x;
    if (sprite->draw.draw_end_x >= start_x + v_w)
        sprite->draw.draw_end_x = start_x + v_w - 1;
    if (sprite->draw.draw_start_x > sprite->draw.draw_end_x)
        return ;
    // get_frame_within_sheet
    cols = (sprite->frame_cols > 0) ? sprite->frame_cols : 1;
    rows = (sprite->frame_rows > 0) ? sprite->frame_rows : 1;
    sprite->draw.frame_col = sprite->frame % cols;
    sprite->draw.frame_row = sprite->frame / cols;
    if (sprite->draw.frame_row >= rows)
        sprite->draw.frame_row = rows - 1;
    sprite->draw.origin_x = sprite->draw.frame_col * sprite->frame_w;
    sprite->draw.origin_y = sprite->draw.frame_row * sprite->frame_h;
    span_x = sprite->draw.draw_end_x - sprite->draw.draw_start_x;
    if (span_x < 1)
        span_x = 1;
    span_y = sprite->draw.draw_end_y - sprite->draw.draw_start_y;
    if (span_y < 1)
        span_y = 1;
    // render sprites
    x = sprite->draw.draw_start_x;
    while (x <= sprite->draw.draw_end_x)
    {
        if (!data->z_buffer || x < 0 || x >= data->z_buffer_size
            || data->z_buffer[x] <= 0.0 || sprite->cam_z >= data->z_buffer[x])
        {
            x++;
            continue;
        }
        u = (double)(x - sprite->draw.draw_start_x) / (double)span_x;
        if (u < 0.0)
            u = 0.0;
        if (u > 1.0)
            u = 1.0;
        tex_x = sprite->draw.origin_x + (int)(u * sprite->frame_w);
        if (tex_x >= sprite->draw.origin_x + sprite->frame_w)
            tex_x = sprite->draw.origin_x + sprite->frame_w - 1;
        if (tex_x >= sprite->sp_tex->width)
            tex_x = sprite->sp_tex->width - 1;
        y = sprite->draw.draw_start_y;
        while (y <= sprite->draw.draw_end_y)
        {
            v = (double)(y - sprite->draw.draw_start_y) / (double)span_y;
            if (v < 0.0)
                v = 0.0;
            if (v > 1.0)
                v = 1.0;
            tex_y = sprite->draw.origin_y + (int)(v * sprite->frame_h);
            if (tex_y >= sprite->draw.origin_y + sprite->frame_h)
                tex_y = sprite->draw.origin_y + sprite->frame_h - 1;
            if (tex_y >= sprite->sp_tex->height)
                tex_y = sprite->sp_tex->height - 1;
            color = texel(sprite->sp_tex, tex_x, tex_y);
            if (!is_transparent(color))
                put_pixel(&data->mlx, x , y, color);
            y++;
        }
        x++;
    }
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

