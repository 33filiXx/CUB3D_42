/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:34:54 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/09/28 11:10:20 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	reset_data(t_cmp_data *cmp_data)
{
	cmp_data->compass = malloc(6 * sizeof(char *));
	cmp_data->compass[0] = ft_strdup(NO);
	cmp_data->compass[1] = ft_strdup(SO);
	cmp_data->compass[2] = ft_strdup(WE);
	cmp_data->compass[3] = ft_strdup(EA);
	cmp_data->compass[4] = ft_strdup(F);
	cmp_data->compass[5] = ft_strdup(C);
}
int	match_in_list(char *s1 , char **s2)
{
    int	i;
    int	j;

	j = 0;
    if (!s1 || !s2)
        return 1;
    while (s2[j])
    {  
        i = 0;
        while (s1 && s2[j] && s1[i] && s2[j][i] && s1[i] == s2[j][i])
	    	i++;
        if(!(s1[i] - s2[j][i]))
			return 0;
        j++;
    }
	return (1);
}

int	set_data(int fd, t_file_data *file_data)
{
	char		*buffer;
	char		**to_be_splited;
	t_cmp_data	*cmp_data;
	int			i;

	(void)file_data;
	cmp_data = malloc(sizeof(t_cmp_data));
	reset_data(cmp_data);
	i = 0;
	while (1)
	{
		buffer = get_next_line(fd);
		if (buffer)
		{
			to_be_splited = ft_split(buffer, "  , ");
			while (to_be_splited[i])
			{
                
			}
		}
	}
}
