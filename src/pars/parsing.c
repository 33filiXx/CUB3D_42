/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:18:41 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/29 17:44:24 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}
int	my_strcmp(const char *s1, const char **s2)
{
	int	i;
	int	j;

	i = 0;
	while (s2[j])
	{
        i = 0;
        while (s2[j][i] && s1[i] && (s1[i] == s2[j][i]))
            i++;
        if(s1[i] - s2[j][i] == 0)
            return s1[i] - s2[j][i];
		j++;
	}
	return (-1);
}

int if_last_point(char *str)
{
    int check;

    check = 0;
    while (*str)
    {
        if(*str == '.')
            check = 1;
        str++;
        if(*str == '.' && check == 1)
            return 1;
    }
    return 0;    
}

int valid_extantion(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '.' && !if_last_point(str + i))
        {
            if (!strcmp(".cub" , str + i))
                return 0;
        }
        i++;
    }
    return 1;
}


int exists(char *str)
{
    int fd;
    
    fd = open(str, O_RDONLY);
    if (fd == -1)
        return -1;
    return fd; 
}

int  punisher(char **av , t_file_data *file_data)
{
    int fd;
    file_data = NULL;

    fd = exists(*av);
    if(valid_extantion(*av))
        return 1;
    if (fd == -1)
        return 1;
    set_data(fd , file_data);
    return 0;
}