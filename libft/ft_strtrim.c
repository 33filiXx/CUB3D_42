/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 13:51:26 by wel-mjiy          #+#    #+#             */
/*   Updated: 2024/11/17 20:41:29 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_set_sup(char *p, char *ptr, int lenght, int i)
{
	int	j;

	j = 0;
	while (i <= lenght)
	{
		p[j] = ptr[i];
		j++;
		i++;
	}
	p[j] = '\0';
	return (p);
}

static char	*ft_set(char *ptr, char const *set)
{
	int		lenght;
	int		i;
	char	*p;

	i = 0;
	while (ptr[i] && ft_strchr(set, ptr[i]))
		i++;
	if (i == (int)ft_strlen(ptr))
	{
		p = malloc(1);
		p[0] = '\0';
		return (p);
	}
	lenght = ft_strlen(ptr) - 1;
	while (ft_strchr(set, ptr[lenght]) && lenght >= 0)
		lenght--;
	p = (char *)malloc((lenght - i + 2) * sizeof(char));
	if (!p)
		return (NULL);
	return (ft_set_sup(p, ptr, lenght, i));
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;

	if (!s1)
		return (NULL);
	if (*s1 == '\0')
		return (ft_strdup(s1));
	if (!set || *set == '\0')
		return (ft_strdup(s1));
	ptr = (char *)s1;
	return (ft_set(ptr, set));
}
