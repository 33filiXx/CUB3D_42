/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:30:05 by wel-mjiy          #+#    #+#             */
/*   Updated: 2024/11/18 16:54:36 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*set_d(char *dst, char *ptr, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dst[i])
	{
		i++;
	}
	while (ptr[j] && i < size - 1)
	{
		dst[i] = ptr[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (dst);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char	*ptr;
	size_t	p1;
	size_t	p2;

	if (!dst && size == 0)
		return (ft_strlen(src));
	ptr = (char *)src;
	p1 = ft_strlen(dst);
	p2 = ft_strlen(src);
	if (size == 0)
		return (p2);
	if (size < p1)
		return (size + p2);
	set_d(dst, ptr, size);
	return (p1 + p2);
}
