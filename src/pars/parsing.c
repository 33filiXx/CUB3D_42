/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:18:41 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/10/17 02:25:54 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int if_last_point(char *str)
{
    while (*str)
    {
        str++;
        if(*str == '.')
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
            if (!ft_strcmp(".cub" , str + i))
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
int get_size(int fd)
{
	int length;
	char *buffer;

	length = 0;
	while (1)
	{
		buffer = get_next_line(fd);
		length++;
		if(!buffer)
			break;
	}
    free(buffer);
	return length;
}
int  punisher(char **av , t_file_data *file_data)
{
    int fd;
    int new_fd;

    fd = exists(*av);
    if (fd == -1)
        return 1;
    if(valid_extantion(*av))
        return 1;
    file_data->map_size = get_size(fd);
    close(fd);
	new_fd = open(*av, O_RDONLY);
	if(new_fd == -1)
		return 1;
    if(is_valid(new_fd , file_data))
        return 1;
    return 0;
}