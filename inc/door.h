/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:08:30 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/14 19:06:24 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOR_H
#define DOOR_H

# include <stdbool.h>
# include "../src/vectorslib/vec.h"

typedef enum s_door_state
{
    DOOR_CLOSED,
    DOOR_OPENING,
    DOOR_OPEN,
    DOOR_CLOSING
}       t_door_state;

typedef struct s_door
{
    int             map_x;
    int             map_y;
    double          progress;
    double          speed;
    t_door_state    state;
    t_vec2          pivot;
    t_vec2          span_closed;
    int             rot_sign;
    bool            has_geom;
}       t_door;

struct s_game_data;

void    door_load_map(struct s_game_data *data);
void    door_toggle(struct s_game_data *data);
void    door_update(struct s_game_data *data, double dt);
bool    door_is_blocking(struct s_game_data *data, int map_x, int map_y);
double	get_now_seconds(void);
t_door *find_door(struct s_game_data *data, int m_y, int m_x);
#endif