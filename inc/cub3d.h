#ifndef CUBE3D_H
# define CUBE3D_H

#	include <fcntl.h>
#	include <math.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <sys/time.h>
#	include <stdbool.h>
	//  #include "../../minilibx-linux/mlx.h"
#	include "../libft/libft.h"
#	include <X11/keysym.h>
#	include <X11/X.h>
#	include "mlx.h"
#	include "../src/vectorslib/vec.h"
#	include "door.h"
# 	include "sprite.h"
#define HEIGHT 1080
#define WIDTH 1920
#define PLAYER_COLLISION_RADIUS 0.2

#  define NO "NO"
#  define SO "SO"
#  define WE "WE"
#  define EA "EA"
#  define F "F"
#  define C "C"


#  define TILE 64
#	define HALF_PI 1.5707963267948966

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
	int map_size;
	int element_size;
	int s_element_size;
	char **map;
	int row;
	int column;
	unsigned int fc;
	unsigned int cc;
}			t_file_data;

typedef struct s_cmp_data
{
	char **compass;
}			t_cmp_data;


// char	**ft_split(char *str, char *charset);
int punisher(char **av , t_file_data *file_data);
int storing(int fd, t_file_data *file_data);
int	set_data(int fd, t_file_data *file_data);
int	ft_strcmp(const char *s1, const char *s2);
void    free_cmp_data(t_cmp_data *cmp);
void    free_double_array(char **arr);
// int	my_strcmp(const char *s1, const char **s2);


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
void	initiate(t_mlx *mlx, t_game_data *game_data);
int game_loop(void *param);
int close_window(void *param);


void render_3d_view(t_game_data *data, int start_x, int view_width, int view_height);
void put_pixel(t_mlx *mlx, int x, int y, int color);
int on_mouse_move(int x, int y, void *param);
void	redraw_map(t_game_data *data);
void    load_texture(t_game_data *data, t_texture *tex, char *path);
int	valid_move(t_game_data *data, double new_x, double new_y);
void    set_current_tex(t_game_data *data, t_st *tex);
void    init_hit_data(t_game_data *data);
void    set_ray_dir_xy(t_game_data *data);
void    tex_ready(int *textures_ready, t_st *tex, t_game_data *data);
int    flip_text_horizontally(t_game_data *data, t_texture *current_tex);
void    draw(t_game_data *data ,t_texture *tex, int view_height, int x, int start_x);
void    set_drawing_ends(t_game_data *data, int view_height);
void    set_texture_coordinations(t_game_data *data);
char    *get_the_right_texture(t_game_data *data);
t_texture *get_current_texture(t_game_data *data, t_texture *tex_no, t_texture *tex_so, t_texture *tex_we, t_texture *tex_ea);
void    get_perp_wall_distance(t_game_data *data);
void    set_horizontal_line_dist(t_game_data *data);
void    set_next_line(t_game_data *data);
int check_bounds(t_game_data *data);
void    set_door_data(t_game_data *data, t_door *door, double dist, double tex_u);
void    dda(t_game_data *data);
void    set_ray_dir(t_game_data *data);
void    set_player_position(t_game_data *data);
void    set_steps(t_game_data *data);
void    set_vertical_line_dist(t_game_data *data);
void    set_line_height(t_game_data *data, int view_height);
int  door_ray_intersection(t_game_data *data, t_door *door,
        double *hit_dist, double *u);
void	init_mouse(t_game_data *data);
void	move_forward(t_game_data *data);
void	move_backwards(t_game_data *data);
void	rotate_right(t_game_data *data);
void	rotate_left(t_game_data *data);
int 	key_press(int keycode, void *param);
int 	key_release(int keycode, void *param);
void	set_moved_flag(t_game_data *data, bool *moved);
void 	rotate_player(t_game_data *data, double angle);
void 	draw_minimap_door(t_game_data *data, t_minimap *minimap, t_door *door);
void	draw_tile(t_game_data *data, int *int_holder, t_minimap *minimap);
void	adjust_dimensions(t_minimap *minimap, t_game_data *data);
void	set_right_color(t_game_data *data, int i , int j , int *color);
void	draw_env(t_game_data *data);
void	drawing_mini_doors(t_mini_draw *mini, t_game_data *data, t_minimap *minimap);
void	drawin_minidoors_helper(t_mini_draw *mini, t_game_data *data, t_minimap *minimap);
void	init_mini_draw(t_mini_draw *mini, t_minimap *minimap, t_door *door);
void	set_colors(t_game_data *data, unsigned int *floor_color, unsigned int * ceiling_color);
void		draw_player(int color, t_game_data *data, t_minimap *minimap);
void	fill_outer_ppixel(t_game_data *data, int i, int j, int floor_color);
void	set_cercle_data(t_game_data *data, t_minimap *minimap, t_cercle *cercle);
void	cleanup_game(t_game_data *data);
void	destroy_textures(t_game_data *data);
void	sprite_sheet_destroy(void);
void 	free_texture_paths(t_file_data *file_data);
void 	destroy_mlx_resources(t_mlx *mlx);
// void 	destroy_texture_image(t_mlx *mlx, t_texture *tex);
t_vec2   ray_origin(t_game_data *data);
t_vec2   ray_direction(t_game_data *data);
int merge_data(t_game_data *game_data, t_file_data *file_data);
void	hit_wall(t_game_data *data);
void	check_doors(t_game_data *data);
unsigned int	texel(t_texture *tex, int x, int y);
bool	render_sprite_x_incr(t_render_sprite *render_s, t_game_data *data, t_sprite *sprite);
bool	ensure_z_buffer(t_game_data *data, int width);
int	*set_ints(int i, int j, int color);
int	get_ty (t_cercle *cercle, t_game_data *data, t_minimap *minimap);
int	blocked_at(t_game_data *data, double x, double y);
void	set_origin_dir(t_game_data *data, t_vec2 *origin, t_vec2 *dir);
void	draw_walls(t_game_data *data, int view_height, int x,
		int start_x, t_texture *current_tex);
void	color_floor_and_ceiling(t_game_data *data, int view_hieght,
						int start_x, int x);
#endif