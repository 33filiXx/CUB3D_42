/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:42:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2024/11/16 18:06:27 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_digit_count(unsigned int n)
{
	int	count;

	count = 1;
	while (n >= 10)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	set_digits(char *p, unsigned int n, int i, int is_negative)
{
	while (i >= is_negative)
	{
		p[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
}

char	*ft_itoa(int c)
{
	unsigned int	n;
	int				digit_count;
	int				is_negative;
	char			*p;

	if (c < 0)
	{
		is_negative = 1;
		n = -c;
	}
	else
	{
		is_negative = 0;
		n = c;
	}
	digit_count = get_digit_count(n);
	p = malloc((digit_count + is_negative + 1) * sizeof(char));
	if (!p)
		return (NULL);
	p[digit_count + is_negative] = '\0';
	if (is_negative)
		p[0] = '-';
	set_digits(p, n, digit_count + is_negative - 1, is_negative);
	return (p);
}
