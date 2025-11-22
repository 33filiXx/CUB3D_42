/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:08:34 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/22 17:23:25 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/door.h"
#include "../../inc/cub3d.h"

#define HALF_PI 1.5707963267948966

static int  map_in_bounds(t_game_data *data, int y, int x)
{
    return (x >= 0 && x < data->map.width && y >= 0 && y < data->map.height);
}

static int  map_is_wall(t_game_data *data, int y, int x)
{
    if (!map_in_bounds(data, y, x))
        return (1);
    return (data->map.grid[y][x] == '1');
}

static int  map_is_walkable(t_game_data *data, int y, int x)
{
    if (!map_in_bounds(data, y, x))
        return (0);
    if (data->map.grid[y][x] == '1')
        return (0);
    return (1);
}

static int  point_hits_wall(t_game_data *data, t_vec2 point)
{
    int x;
    int y;

    x = (int)floor(point.x);
    y = (int)floor(point.y);
    if (!map_in_bounds(data, y, x))
        return (1);
    return (data->map.grid[y][x] == '1');
}

static void set_candidate(t_vec2 center, t_vec2 span_dir, int hinge_sign,
        t_vec2 *pivot, t_vec2 *span)
{
    *pivot = vec2_add(center, vec2_scale(span_dir, 0.5 * hinge_sign));
    if (hinge_sign < 0)
        *span = span_dir;
    else
        *span = vec2_scale(span_dir, -1.0);
}

static int  evaluate_geometry(t_game_data *data, t_vec2 pivot, t_vec2 span,
        int rot_sign)
{
    t_vec2  span_open;
    t_vec2  sample_mid;
    t_vec2  sample_tip;

    span_open = vec2_rotate(span, rot_sign * HALF_PI);
    sample_mid = vec2_add(pivot, vec2_scale(span_open, 0.5));
    sample_tip = vec2_add(pivot, vec2_scale(span_open, 0.9));
    if (point_hits_wall(data, sample_mid) || point_hits_wall(data, sample_tip))
        return (0);
    return (1);
}

static void configure_door_geometry(t_game_data *data, t_door *door)
{
    t_vec2      center;
    t_vec2      span_dir;
    int         vertical_open;
    int         horizontal_open;
    int         best_score;
    t_vec2      best_pivot;
    t_vec2      best_span;
    int         best_rot;
    int         hinge_signs[2];
    int         rot_signs[2];
    int         i;
    int         j;

    center = vec2_new(door->map_x + 0.5, door->map_y + 0.5);
    vertical_open = map_is_walkable(data, door->map_y - 1, door->map_x)
        + map_is_walkable(data, door->map_y + 1, door->map_x);
    horizontal_open = map_is_walkable(data, door->map_y, door->map_x - 1)
        + map_is_walkable(data, door->map_y, door->map_x + 1);
    if (vertical_open > horizontal_open)
        span_dir = vec2_new(1.0, 0.0);
    else if (horizontal_open > vertical_open)
        span_dir = vec2_new(0.0, 1.0);
    else
        span_dir = vec2_new(1.0, 0.0);
    hinge_signs[0] = -1;
    hinge_signs[1] = 1;
    rot_signs[0] = 1;
    rot_signs[1] = -1;
    best_score = -1;
    best_rot = 1;
    best_pivot = center;
    best_span = span_dir;
    i = 0;
    while (i < 2)
    {
        t_vec2  pivot;
        t_vec2  span;
        int     hinge_sign;
        int     wall_bonus;

        hinge_sign = hinge_signs[i];
        set_candidate(center, span_dir, hinge_sign, &pivot, &span);
        wall_bonus = 0;
        if (span_dir.x != 0.0 && map_is_wall(data, door->map_y,
                door->map_x + hinge_sign))
            wall_bonus = 1;
        if (span_dir.y != 0.0 && map_is_wall(data, door->map_y + hinge_sign,
                door->map_x))
            wall_bonus = 1;
        j = 0;
        while (j < 2)
        {
            int rot_sign;
            int score;

            rot_sign = rot_signs[j];
            if (!evaluate_geometry(data, pivot, span, rot_sign))
            {
                j++;
                continue;
            }
            score = wall_bonus;
            if (score > best_score)
            {
                best_score = score;
                best_pivot = pivot;
                best_span = span;
                best_rot = rot_sign;
            }
            j++;
        }
        i++;
    }
    if (best_score < 0)
    {
        set_candidate(center, span_dir, -1, &best_pivot, &best_span);
        best_rot = 1;
    }
    door->pivot = best_pivot;
    door->span_closed = best_span;
    door->rot_sign = best_rot;
    door->has_geom = true;
}

double	get_now_seconds(void)
{
    struct timeval	tv;

    if (gettimeofday(&tv, NULL) != 0)
        return (0.0);
    return ((double)tv.tv_sec + (double)tv.tv_usec / 1000000.0);
}

static bool append_to_doors(t_game_data *data, int map_y, int map_x)
{
    t_door *tmp;
    t_door *door;
    size_t  new_count;

    new_count = data->door_count + 1;
    tmp = realloc(data->doors, sizeof(t_door) * new_count);
    if (!tmp)
        return (false);
    data->doors = tmp;
    door = &data->doors[new_count - 1];
    door->map_x = map_x;
    door->map_y = map_y;
    door->progress = 0.0;
    door->speed = 0.1;
    door->state = DOOR_CLOSED;
    door->pivot = vec2_zero();
    door->span_closed = vec2_new(1.0, 0.0);
    door->rot_sign = 1;
    door->has_geom = false;
    data->door_count = new_count;
    return (true);
}

t_door *find_door(t_game_data *data, int m_y, int m_x)
{
    size_t i = 0;
    while (i < data->door_count)
    {
        if (data->doors[i].map_x == m_x && data->doors[i].map_y == m_y)
            return &data->doors[i];
        i++;
    }
    return NULL;
}

static bool get_front_cell(t_game_data *data, int *out_y, int *out_x)
{
    double target_x;
    double target_y;
    const double reach = 1.0;

    target_x = data->player.pos.x + data->player.dir.x * reach;
    target_y = data->player.pos.y + data->player.dir.y * reach;
    int map_x = (int)target_x;
    int map_y = (int)target_y;

    if (map_x < 0 || map_x >= data->map.width
        || map_y < 0 || map_y >= data->map.height)
        return false;
    *out_x = map_x;
    *out_y = map_y;
    return true;
}

void    door_load_map(t_game_data *data)
{
    int y;
    int x;

    if (data->doors)
    {
        free(data->doors);
        data->doors = NULL;
    }
    data->door_count = 0;
    y = 0;
    while (y < data->map.height)
    {
        x = 0;
        while (x < data->map.width)
        {
            if (data->map.grid[y][x] == 'D')
            {
                if (!append_to_doors(data, y, x))
                    return ;
                configure_door_geometry(data, &data->doors[data->door_count - 1]);
            }
            x++;
        }
        y++;
    }
}

void door_toggle(t_game_data *data)
{
    int map_y;
    int map_x;
    t_door *door;

    if (!get_front_cell(data, &map_y, &map_x))
        return;
    door = find_door(data, map_y, map_x);
    if (!door)
        return;

    if (door->state == DOOR_CLOSED || door->state == DOOR_CLOSING)
    {
        if (door->state == DOOR_CLOSED)
            door->progress = 0.0;
        door->state = DOOR_OPENING;
    }
    else if (door->state == DOOR_OPEN || door->state == DOOR_OPENING)
    {
        if (door->state == DOOR_OPEN)
            door->progress = 1.0;
        door->state = DOOR_CLOSING;
    }
}

void    door_update(t_game_data *data, double dt)
{
    bool    changed;
    size_t  i;
    t_door  *door;

    if (dt < 0)
        dt = 0;
    changed = false;
    i = 0;
    while (i < data->door_count)
    {
        door = &data->doors[i];
        if (door->state == DOOR_OPENING)
        {
            door->progress += door->speed * dt;
            if (door->progress >= 1.0)
            {
                door->progress = 1.0;
                door->state = DOOR_OPEN;
            }
            changed = true;
        }
        else if (door->state == DOOR_CLOSING)
        {
            door->progress -= door->speed * dt;
            if (door->progress <= 0.0)
            {
                door->progress = 0.0;
                door->state = DOOR_CLOSED;
            }
            changed = true;
        }
        i++;
    }
    if (changed)
        redraw_map(data);
}

bool    door_is_blocking(t_game_data *data, int map_x, int map_y)
{
    t_door *door = find_door(data, map_y, map_x);
    if (!door)
        return (false);
    if (door->state == DOOR_OPEN)
        return (false);
    if (door->state == DOOR_OPENING)
        return (door->progress < 0.95);
    if (door->state == DOOR_CLOSING)
        return (true);
    return (true);
        
}