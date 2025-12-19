/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/19 15:43:19 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"
#include <math.h>

double	vec2_distance(t_vec2 a, t_vec2 b)
{
	return (vec2_length(vec2_sub(a, b)));
}

t_vec2	vec2_lerp(t_vec2 a, t_vec2 b, double t)
{
	return (vec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t));
}

double	vec2_angle(t_vec2 v)
{
	return (atan2(v.y, v.x));
}
