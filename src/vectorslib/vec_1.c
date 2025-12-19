/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 16:05:47 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"
#include <math.h>

double	vec2_cross(t_vec2 a, t_vec2 b)
{
	return (a.x * b.y - a.y * b.x);
}

double	vec2_length(t_vec2 v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

t_vec2	vec2_normalize(t_vec2 v)
{
	double	len;

	len = vec2_length(v);
	if (len == 0)
		return (vec2(0, 0));
	return (vec2(v.x / len, v.y / len));
}

t_vec2	vec2_rotate(t_vec2 v, double angle)
{
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	return (vec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a));
}

t_vec2	vec2_perpendicular(t_vec2 v)
{
	return (vec2(-v.y, v.x));
}
