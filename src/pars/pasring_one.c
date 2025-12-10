/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pasring_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 02:03:30 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/09 02:43:00 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	if_last_point(char *str)
{
	while (*str)
	{
		str++;
		if (*str == '.')
			return (1);
	}
	return (0);
}

int	valid_extantion(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '.' && !if_last_point(str + i))
		{
			if (!ft_strcmp(".cub", str + i))
				return (0);
		}
		i++;
	}
	return (1);
}

int	exists(char *str)
{
	int	fd;

	fd = open(str, O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
}
