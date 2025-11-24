#include "vec.h"

t_vec2	vec2_new(double x, double y)
{
	t_vec2	v;

	v.x = x;
	v.y = y;
	return (v);
}

t_vec2	vec2_zero(void)
{
	return (vec2_new(0.0, 0.0));
}

t_vec2	vec2_add(t_vec2 a, t_vec2 b)
{
	return (vec2_new(a.x + b.x, a.y + b.y));
}

t_vec2	vec2_sub(t_vec2 a, t_vec2 b)
{
	return (vec2_new(a.x - b.x, a.y - b.y));
}

t_vec2	vec2_scale(t_vec2 v, double scalar)
{
	return (vec2_new(v.x * scalar, v.y * scalar));
}

t_vec2	vec2_negate(t_vec2 v)
{
	return (vec2_new(-v.x, -v.y));
}

double	vec2_magnitude(t_vec2 v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

double	vec2_magnitude_squared(t_vec2 v)
{
	return (v.x * v.x + v.y * v.y);
}

t_vec2	vec2_normalize(t_vec2 v)
{
	double	mag;

	mag = vec2_magnitude(v);
	if (mag == 0.0)
		return (vec2_zero());
	return (vec2_new(v.x / mag, v.y / mag));
}

double	vec2_dot(t_vec2 a, t_vec2 b)
{
	return (a.x * b.x + a.y * b.y);
}

double	vec2_cross(t_vec2 a, t_vec2 b)
{
	return (a.x * b.y - a.y * b.x);
}

double	vec2_distance(t_vec2 a, t_vec2 b)
{
	return (vec2_magnitude(vec2_sub(b, a)));
}

double	vec2_angle_between(t_vec2 a, t_vec2 b)
{
	double	dot;
	double	mag_a;
	double	mag_b;

	mag_a = vec2_magnitude(a);
	mag_b = vec2_magnitude(b);
	if (mag_a == 0.0 || mag_b == 0.0)
		return (0.0);
	dot = vec2_dot(a, b);
	return (acos(dot / (mag_a * mag_b)));
}

t_vec2	vec2_rotate(t_vec2 v, double angle)
{
	t_vec2	result;
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	result.x = v.x * cos_a - v.y * sin_a;
	result.y = v.x * sin_a + v.y * cos_a;
	return (result);
}

t_vec2	vec2_perpendicular(t_vec2 v)
{
	return (vec2_new(-v.y, v.x));
}

int	vec2_equals(t_vec2 a, t_vec2 b, double epsilon)
{
	return (fabs(a.x - b.x) < epsilon && fabs(a.y - b.y) < epsilon);
}

t_vec2	vec2_lerp(t_vec2 a, t_vec2 b, double t)
{
	return (vec2_add(vec2_scale(a, 1.0 - t), vec2_scale(b, t)));
}
