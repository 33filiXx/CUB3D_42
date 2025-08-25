/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:42:55 by wel-mjiy          #+#    #+#             */
/*   Updated: 2024/11/18 15:47:35 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	unsigned char	p;
	size_t			i;

	ptr = (unsigned char *)b;
	p = (unsigned char)c;
	i = 0;
	while (i < len)
	{
		ptr[i] = p;
		i++;
	}
	return (b);
}
