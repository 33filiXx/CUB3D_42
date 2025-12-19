/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/10 16:05:47 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

t_vec2	vec2(double x, double y)
{
	t_vec2	v;

	v.x = x;
	v.y = y;
	return (v);
}

t_vec2	vec2_add(t_vec2 a, t_vec2 b)
{
	return (vec2(a.x + b.x, a.y + b.y));
}

t_vec2	vec2_sub(t_vec2 a, t_vec2 b)
{
	return (vec2(a.x - b.x, a.y - b.y));
}

t_vec2	vec2_scale(t_vec2 v, double s)
{
	return (vec2(v.x * s, v.y * s));
}

double	vec2_dot(t_vec2 a, t_vec2 b)
{
	return (a.x * b.x + a.y * b.y);
}
