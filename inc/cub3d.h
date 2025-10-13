#ifndef CUBE3D_H
# define CUBE3D_H

# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
// #include "../../minilibx-linux/mlx.h"
# include "../libft/libft.h"

#  define NO "NO"
#  define SO "SO"
#  define WE "WE"
#  define EA "EA"
#  define F "F"
#  define C "C"



typedef struct s_file_data
{
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
	int		floor_color[3];
	int		ceiling_color[3];
	char *hexa_ceiling_color;
	char *hexa_f_color;
}			t_file_data;

typedef struct s_cmp_data
{
	char **compass;
}			t_cmp_data;


// char	**ft_split(char *str, char *charset);
int punisher(char **av , t_file_data *file_data);
int	set_data(int fd, t_file_data *file_data);
int	ft_strcmp(const char *s1, const char *s2);
// int	my_strcmp(const char *s1, const char **s2);



#endif