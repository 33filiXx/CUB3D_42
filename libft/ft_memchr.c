/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:49:43 by wel-mjiy          #+#    #+#             */
/*   Updated: 2024/11/17 21:23:15 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	p;
	size_t			i;

	ptr = (unsigned char *)s;
	p = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == p)
		{
			return (&ptr[i]);
		}
		i++;
	}
	return (0);
}
