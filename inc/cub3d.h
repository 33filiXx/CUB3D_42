#ifndef CUBE3D_H
# define CUBE3D_H

#	include <fcntl.h>
#	include <math.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <sys/time.h>
#	include <stdbool.h>
//	 #include "../../minilibx-linux/mlx.h"
#	include "../libft/libft.h"
#	include <X11/keysym.h>
#	include <X11/X.h>
#	include <mlx.h>
#	include "../src/vectorslib/vec.h"
#	include "door.h"
# 	include "sprite.h"
#	define NO "NO"
#	define SO "SO"
#	define WE "WE"
#	define EA "EA"
#	define F "F"
#	define C "C"

#  define TILE 64
/*
void func(your struct , my struct)
{
	my struct map 
}
*/

typedef enum s_hit_kind
{
	HIT_NONE,
	HIT_WALL,
	HIT_DOOR
}			t_hit_kind;


typedef struct s_cercle
{
	int c_x;
	int c_y;
	int	r;
}			t_cercle;
typedef struct s_file_data
{
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
	int		floor_color[3];
	int		ceiling_color[3];
	unsigned int fc;
	unsigned int cc;
}			t_file_data;


typedef struct s_texture
{
    void *img;
	void *mlx_connection;
    char *addr;
    int width;
    int height;
    int bpp;
    int line_len;
    int endian;
	int tex_x;
	int tex_y;
	bool            has_transparency;
	unsigned int    transparent_color;
} t_texture;

typedef struct s_minimap
{
	int	mini_width;
	int	mini_height;
	int	padding;
	int	mini_tile;
}		t_minimap;

typedef struct s_nt
{
	t_texture tex_no;
	t_texture tex_so;
	t_texture tex_we;
	t_texture tex_ea;
	t_texture door_tex;
	t_texture *current_tex;
}	t_st;

typedef struct s_cmp_data
{
	char **compass;
}			t_cmp_data;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}			t_map;

typedef struct s_door_geo
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
	t_vec2  pivot;
    t_vec2  span;
    int     hinge_sign;
    int     wall_bonus;
}				t_door_geo;

typedef struct s_player
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
	double	move_speed;
	double	rot_speed;
	int moving_forward;
	int moving_backward;
	int rotating_left;
	int rotating_right;
}			t_player;

typedef struct s_raycast
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	double		wall_x;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int draw_start_raw;
	t_hit_kind kind;
	t_door *hit_door;
	double door_progress;
}			t_raycast;

typedef struct s_mlx
{
	void		*mlx_connection;
	void		*mlx_win;
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}			t_mlx;

typedef struct s_mouse
{
	int c_x;
	bool has_prev_pos;
	double prev_delta;
}			t_mouse;

typedef struct s_frame
{
	int span_x;
    int span_y;
	int cols;
    int rows;
}			t_frame;

typedef struct s_render_sprite
{
	int x;
    int y;
    int tex_x;
    int tex_y;
    double u;
    double v;
    unsigned int color;
}			t_render_sprite;

typedef struct s_minimap_draw
{
	t_vec2 span;
	double scale;
	double pivot_x;
	double pivot_y;
	double step;
	double t;
	int	steps;
	int	k;
	t_vec2 pt;
	int	px;
	int	py;
	int	dy;
	int	dx;
}				t_mini_draw;


typedef struct s_game_data
{
    t_mlx       mlx;
    t_map       map;
    t_player    player;
    t_file_data file_data;
	t_mouse		mouse;
	t_raycast	rc;
	t_door		*doors;
	size_t		door_count;
	double		last_time;
	int          sprite_count;
	int			v_sprite_counter;
	t_sprite	*sprite;
	double		*z_buffer;
	int		z_buffer_size;
}   t_game_data;

// char	**ft_split(char *str, char *charset);
int punisher(char **av , t_file_data *file_data);
int set_data(int fd , t_file_data *file_data);
int	ft_strcmp(const char *s1, const char *s2);
// int	my_strcmp(const char *s1, const char **s2);
void render_3d_view(t_game_data *data, int start_x, int view_width, int view_height);
void put_pixel(t_mlx *mlx, int x, int y, int color);
int on_mouse_move(int x, int y, void *param);
void	redraw_map(t_game_data *data);
void    load_texture(t_game_data *data, t_texture *tex, char *path);
int	valid_move(t_game_data *data, double new_x, double new_y);
void    get_z_buffer(t_game_data *data);
void    free_z_buffer(t_game_data *data);
t_sprite    **sort_sprites(t_game_data *data);
void    sprite_camera_transform(t_game_data *data, t_sprite *sprite);
void    sprite_update_one(t_game_data *data, t_sprite *sprite, double dt);
void    sprite_animate(t_sprite *sprite, double dt);
void sprite_load_map(t_game_data *data);
void    sprite_sheet_init(t_game_data *data, t_sprite *sprite);
void sprite_update_all(t_game_data *data, double dt);
void sprite_render_all(t_game_data *data, int start_x, int view_w, int view_h);
void	sprite_draw(t_game_data *data, t_sprite *sprite, int start_x, int v_w, int v_h);

#endif