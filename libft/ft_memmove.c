/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 23:31:18 by wel-mjiy          #+#    #+#             */
/*   Updated: 2024/11/18 14:36:02 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dest;
	unsigned char	*sourc;
	size_t			i;

	if (dst == src)
		return (dst);
	dest = (unsigned char *)dst;
	sourc = (unsigned char *)src;
	i = 0;
	if (!dest && !sourc && len > 0)
		return (NULL);
	if (dest > sourc)
	{
		while (len--)
			dest[len] = sourc[len];
	}
	else
	{
		while (i < len)
		{
			dest[i] = sourc[i];
			i++;
		}
	}
	return (dst);
}
