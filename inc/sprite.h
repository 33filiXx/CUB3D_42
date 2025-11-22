/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 20:17:17 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/19 18:06:02 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

# include <stdbool.h>
# include "../src/vectorslib/vec.h"

typedef struct s_texture		 t_texture;

typedef struct s_draw_sprite
{
    int draw_start_y;
    int draw_end_y;
    int draw_start_x;
    int draw_end_x;
    double inv_z;
    int screen_x;
    int sprite_height;
    int sprite_width;
    int	frame_col;
    int	frame_row;
    int origin_x;
    int origin_y;
}			t_draw_sprite;

typedef enum s_sprite_state
{
    SPRITE_IDLE,
    SPRITE_CHASE,
}       t_sprite_state;

typedef struct s_sprite
{
    int             map_x;
    int             map_y;
    t_vec2          position;
    t_vec2          velocity;
    double          speed;
    double          dist_cam;
    double          cam_x;
    double          cam_z;
    int             frame;
    int             frame_w;
    int             frame_h;
    int             frame_cols;
    int             frame_rows;
    double          frame_time;
    double          frame_duration;
    int             frame_count;
    t_texture       *sp_tex;
    t_sprite_state  state;
    t_draw_sprite   draw;
    bool            visible;
}       t_sprite;


#endif

