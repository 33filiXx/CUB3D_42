/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:18:41 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/10 15:48:01 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	get_size(int fd, t_file_data *file_data)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int if_last_point(char *str)
{
    printf("last  :   %s\n" , str);
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
            printf("valid :    %s\n" , str + i);
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

int  punisher(char **av , t_file_data *file_data)
{
    int fd;
    file_data = NULL;

	fd = exists(*av);
	if (fd == -1)
		return (1);
	if (valid_extantion(*av))
		return (1);
	get_size(fd, file_data);
	file_data->s_element_size = 0;
	close(fd);
	new_fd = open(*av, O_RDONLY);
	if (new_fd == -1)
		return (1);
	if (storing(new_fd, file_data))
		return (1);
	return (0);
}
