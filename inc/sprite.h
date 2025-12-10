/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 20:17:17 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 16:05:47 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

# include <stdbool.h>
# include "../src/vectorslib/vec.h"
# include "../inc/cub3d.h"

# define SPRITE_FRAME_SIZE 32
# define SPRITE_FRAME_COUNT 3

typedef struct s_texture	t_texture;

struct						s_game_data;
struct						s_frame;
struct						s_render_sprite;

typedef struct s_draw_sprite
{
	int		draw_start_y;
	int		draw_end_y;
	int		draw_start_x;
	int		draw_end_x;
	double	inv_z;
	int		screen_x;
	int		sprite_height;
	int		sprite_width;
	int		frame_col;
	int		frame_row;
	int		origin_x;
	int		origin_y;
}	t_draw_sprite;

typedef enum s_sprite_state
{
	SPRITE_IDLE,
	SPRITE_CHASE,
}	t_sprite_state;

typedef struct s_sprite
{
	int				map_x;
	int				map_y;
	t_vec2			position;
	t_vec2			velocity;
	double			speed;
	double			dist_cam;
	double			cam_x;
	double			cam_z;
	int				frame;
	int				frame_w;
	int				frame_h;
	int				frame_cols;
	int				frame_rows;
	double			frame_time;
	double			frame_duration;
	int				frame_count;
	t_texture		*sp_tex;
	t_sprite_state	state;
	t_draw_sprite	draw;
	bool			visible;
}	t_sprite;

struct						s_infos;

struct s_frame				get_frame_within_sheet(t_sprite *sprite);
void						render_helper(t_sprite *sprite,
								struct s_frame *frame,
								struct s_game_data *data,
								struct s_render_sprite *render_s);
void						render_sprites(t_sprite *sprite,
								struct s_frame *frame,
								struct s_game_data *data);
void						set_cam_z(t_sprite *sprite, t_vec2 rel);
void						project_to_screen(t_sprite *sprite, int start_x,
								int v_w, int v_h);
void						clamp_drawing_bounds(t_sprite *sprite, int start_x,
								int v_w, int v_h);
int							get_visible_s_len(struct s_game_data *data);
int							collect_visible_sprites(struct s_game_data *data,
								t_sprite ***visible_sprites);
void						swap(t_sprite **v_sprites, int i, int j);
void						free_sprites(t_sprite **visible_sprites, int len);
void						sprite_try_move(struct s_game_data *data,
								t_sprite *sprite, t_vec2 step);
int							move_to_target(struct s_game_data *data,
								t_sprite *sprite, t_vec2 step);
unsigned int				texel(t_texture *tex, int x, int y);
bool						texture_is_transparent(t_texture *tex,
								unsigned int color);
void						set_frame(t_sprite *sprite);
void						append_to_sprite(struct s_game_data *data,
								int x, int y);
void						get_z_buffer(struct s_game_data *data);
void						free_z_buffer(struct s_game_data *data);
t_sprite					**sort_sprites(struct s_game_data *data);
void						sprite_camera_transform(struct s_game_data *data,
								t_sprite *sprite);
void						sprite_update_one(struct s_game_data *data,
								t_sprite *sprite, double dt);
void						sprite_animate(t_sprite *sprite, double dt);
void						sprite_load_map(struct s_game_data *data);
void						sprite_sheet_init(struct s_game_data *data,
								t_sprite *sprite);
void						sprite_update_all(struct s_game_data *data,
								double dt);
void						sprite_render_all(struct s_game_data *data,
								int start_x, int view_w, int view_h);
void						sprite_draw(struct s_game_data *data,
								t_sprite *sprite, struct s_infos infos);

#endif
