/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 12:28:20 by wel-mjiy          #+#    #+#             */
/*   Updated: 2024/11/18 16:16:44 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	int		total_size;
	char	*p;

	if (size && SIZE_MAX / size < nmemb)
		return (NULL);
	total_size = nmemb * size;
	if (nmemb < 0)
		return (NULL);
	p = (char *)malloc(total_size);
	if (!p)
		return (NULL);
	if (nmemb == 0 || size == 0)
		total_size = 1;
	return (ft_memset(p, 0, total_size));
}
