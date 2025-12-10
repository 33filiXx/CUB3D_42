/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 16:17:54 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_H
# define VEC_H

# include <math.h>

typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

/* vec.c */
t_vec2	vec2(double x, double y);
t_vec2	vec2_add(t_vec2 a, t_vec2 b);
t_vec2	vec2_sub(t_vec2 a, t_vec2 b);
t_vec2	vec2_scale(t_vec2 v, double s);
double	vec2_dot(t_vec2 a, t_vec2 b);

/* vec_1.c */
double	vec2_cross(t_vec2 a, t_vec2 b);
double	vec2_length(t_vec2 v);
t_vec2	vec2_normalize(t_vec2 v);
t_vec2	vec2_rotate(t_vec2 v, double angle);
t_vec2	vec2_perpendicular(t_vec2 v);

/* vec_2.c */
double	vec2_distance(t_vec2 a, t_vec2 b);
t_vec2	vec2_lerp(t_vec2 a, t_vec2 b, double t);
double	vec2_angle(t_vec2 v);

#endif
