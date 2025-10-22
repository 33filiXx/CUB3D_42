#ifndef CUBE3D_H
# define CUBE3D_H

# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
// #include "../../minilibx-linux/mlx.h"
# include "../libft/libft.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <mlx.h>
# include "../src/vectorslib/vec.h"
#  define NO "NO"
#  define SO "SO"
#  define WE "WE"
#  define EA "EA"
#  define F "F"
#  define C "C"

#  define TILE 64

typedef struct s_file_data
{
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
	int		floor_color[3];
	int		ceiling_color[3];
}			t_file_data;

typedef struct s_cmp_data
{
	char **compass;
}			t_cmp_data;

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

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}			t_map;

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

typedef struct s_game_data
{
    t_mlx       mlx;
    t_map       map;
    t_player    player;
    t_file_data file_data;
	t_raycast	rc;
}   t_game_data;

// char	**ft_split(char *str, char *charset);
int punisher(char **av , t_file_data *file_data);
int set_data(int fd , t_file_data *file_data);
int	ft_strcmp(const char *s1, const char *s2);
// int	my_strcmp(const char *s1, const char **s2);



#endif