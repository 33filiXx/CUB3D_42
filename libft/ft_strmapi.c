/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 01:56:40 by wel-mjiy          #+#    #+#             */
/*   Updated: 2024/11/17 21:29:36 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*newchangedarray;

	i = 0;
	if (!s)
		return (NULL);
	newchangedarray = malloc(ft_strlen(s) + 1 * sizeof(char));
	if (!newchangedarray)
		return (NULL);
	if (!f)
		return (newchangedarray);
	while (s[i])
	{
		newchangedarray[i] = f(i, s[i]);
		i++;
	}
	newchangedarray[i] = '\0';
	return (newchangedarray);
}
