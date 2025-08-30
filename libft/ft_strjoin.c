/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:23:34 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/28 17:01:53 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcat(char const *s1, char const *s2, char *dest)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		dest[i] = s2[j];
		j++;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		ls2;
	int		ls1;
	char	*p;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	p = (char *)malloc((ls1 + ls2 + 1) * sizeof(char));
	if (!p)
		return (0);
	return (ft_strcat(s1, s2, p));
}
