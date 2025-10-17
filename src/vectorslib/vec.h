#ifndef VEC_H
# define VEC_H

# include <math.h>

typedef struct s_vec2
{
	double	x;
	double	y;
}			t_vec2;

// Vector creation
t_vec2	vec2_new(double x, double y);
t_vec2	vec2_zero(void);

// Basic arithmetic operations
t_vec2	vec2_add(t_vec2 a, t_vec2 b);
t_vec2	vec2_sub(t_vec2 a, t_vec2 b);
t_vec2	vec2_scale(t_vec2 v, double scalar);
t_vec2	vec2_negate(t_vec2 v);

// Vector properties
double	vec2_magnitude(t_vec2 v);
double	vec2_magnitude_squared(t_vec2 v);
t_vec2	vec2_normalize(t_vec2 v);

// Vector products
double	vec2_dot(t_vec2 a, t_vec2 b);
double	vec2_cross(t_vec2 a, t_vec2 b);

// Geometric operations
double	vec2_distance(t_vec2 a, t_vec2 b);
double	vec2_angle_between(t_vec2 a, t_vec2 b);
t_vec2	vec2_rotate(t_vec2 v, double angle);
t_vec2	vec2_perpendicular(t_vec2 v);

// Utility functions
int		vec2_equals(t_vec2 a, t_vec2 b, double epsilon);
t_vec2	vec2_lerp(t_vec2 a, t_vec2 b, double t);

#endif
