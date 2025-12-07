/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 17:05:41 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/12/07 20:48:36 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static char *dup_trimmed_token(char *token)
{
    char *trimmed;
    char *result;

    if (!token)
        return (NULL);
    trimmed = ft_strtrim(token, " \t\n\r");
    if (!trimmed)
        return (NULL);
    result = ft_strdup(trimmed);
    free(trimmed);
    return (result);
}

void cleanup_inside_set_data(t_cmp_data *cmp_data, char **already_checked, char **to_be_splited, char *buffer)
{
    if (cmp_data)
        free_cmp_data(cmp_data);
    if (already_checked)
        free_double_array(already_checked);
    if (to_be_splited)
        free_double_array(to_be_splited);
    free(buffer);
}

static void reset_data(t_cmp_data *cmp_data)
{
    cmp_data->compass = malloc(7 * sizeof(char *));
    cmp_data->compass[0] = ft_strdup(NO);
    cmp_data->compass[1] = ft_strdup(SO);
    cmp_data->compass[2] = ft_strdup(WE);
    cmp_data->compass[3] = ft_strdup(EA);
    cmp_data->compass[4] = ft_strdup(F);
    cmp_data->compass[5] = ft_strdup(C);
    cmp_data->compass[6] = NULL;
}
static void reset_data_helper(t_helper_data *helper_data , int flag)
{
    
    if (flag)
    {
        // helper_data->tmp = malloc(4);
        helper_data->i = 1;
    }
    else
        helper_data->i = 0;
    helper_data->j = 0;
    helper_data->p = 0;
    helper_data->value = 0;
    helper_data->check = 0;
    helper_data->checked = 0;
}

int is_deff_line(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] != '\t' && str[i] != '\n' && str[i] != ' ')
            return 1;
        i++;
    }
    return 0;
}

int match_in_list_helper(char **already_checked, t_helper_data *helper_data, char *s1, char **s2)
{
    if (!(s1[helper_data->i] - s2[helper_data->j][helper_data->i]))
    {
        if (!already_checked[helper_data->checked])
        {
            already_checked[helper_data->checked] = ft_strdup(s1);
            return (0);
        }
        while (already_checked[helper_data->checked])
        {
            if (!ft_strcmp(s1, already_checked[helper_data->checked]))
                return (2);
            (helper_data->checked)++;
        }
        already_checked[helper_data->checked] = ft_strdup(s1);
        return (0);
    }
    return 1;
}
int match_in_list(char *s1, char **s2, char **already_checked)
{

    t_helper_data *helper_data;

    helper_data = malloc(sizeof(helper_data));
    reset_data_helper(helper_data , 0);
    if (!s1 || !s2)
        return (1);
    while (s2[helper_data->j])
    {
        helper_data->i = 0;
        while (s1 && s2[helper_data->j] && s1[helper_data->i] && s2[helper_data->j][helper_data->i] && s1[helper_data->i] == s2[helper_data->j][helper_data->i])
            (helper_data->i)++;
        helper_data->value = match_in_list_helper(already_checked, helper_data, s1, s2);
        if (helper_data->value != 1)
            return (free(helper_data) ,helper_data->value );
        (helper_data->j)++;
    }
    if (is_deff_line(s1))
        return (free(helper_data) , 1);
    free(helper_data);
    return 0;
}

int comma_length_checker_helper(char *str , int *i , int *counter , int *comma)
{
    while (str[*i])
    {
        if (str[*i] >= '0' && str[*i] <= '9')
        {
            (*counter)++;
            if (*counter > 3)
            {
                return (1);
            }
        }
        else if (str[*i] == ',')
        {
            *counter = 0;
            (*comma)++;
        }
        if (*comma > 2)
            return (1);
        (*i)++;
    }
    return 0;
}

int comma_length_checker(char *str)
{
    int(i), (counter), (comma);

    i = 0;
    counter = 0;
    comma = 0;
    while (str[i])
    {
        if (str[i] == ',' && str[i + 1] && str[i + 1] == ',')
            return (1);
        i++;
    }
    i = 0;
    if(comma_length_checker_helper(str , &i ,& counter , &comma) == 1)
        return 1;
    return (0);
}

int next_one(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == ',')
            return 1;
        else if ((str[i] >= '0' && str[i] <= '9'))
            return 0;
        i++;
    }
    return 0;
}







// int handle_c()
// {
// if (who_know == 'F')
//         {
//             if (buffer[helper_data->i] >= '0' && buffer[helper_data->i] <= '9')
//             {
//                 tmp[(helper_data->p)++] = buffer[helper_data->i];
//                 helper_data->check = 1;
//             }
//             else if (buffer[helper_data->i] == ',')
//             {
//                 tmp[helper_data->p] = '\0';
//                 helper_data->check = 0;
//                 if (ft_atoi(tmp) > 255)
//                 {
//                     free(tmp);
//                     return 1;
//                 }
//                 file_data->floor_color[(helper_data->j)++] = ft_atoi(tmp);
//                 free(tmp);
//                 tmp = malloc(4);
//                 (helper_data->i)++;
//                 helper_data->p = 0;
//                 continue;
//             }
//             else if (buffer[helper_data->i] != ' ')
//             {
//                 free(tmp);
//                 return 1;
//             }
//             if (helper_data->check && buffer[helper_data->i] == ' ' && !next_one(buffer + helper_data->i))
//             {
//                 free(tmp);
//                 return 1;
//             }
//         }


    
// }






static int	handle_comma(char **tmp, t_helper_data *h, int *arr)
{
	(*tmp)[h->p] = '\0';
	h->check = 0;
	if (ft_atoi(*tmp) > 255)
	{
		free(*tmp);
		return (1);
	}
	arr[(h->j)++] = ft_atoi(*tmp);
	free(*tmp);
	*tmp = malloc(4);
	h->p = 0;
	return (0);
}

static int	parse_step(char *buf, char **tmp, t_helper_data *h, int *arr)
{
	if (buf[h->i] >= '0' && buf[h->i] <= '9')
	{
		(*tmp)[(h->p)++] = buf[h->i];
		h->check = 1;
	}
	else if (buf[h->i] == ',')
		return (handle_comma(tmp, h, arr));
	else if (buf[h->i] != ' ')
	{
		free(*tmp);
        free(h);
		return (1);
	}
	if (h->check && buf[h->i] == ' ' && !next_one(buf + h->i))
	{
		free(*tmp);
		return (1);
	}
	return (0);
}

static int	finalize_store(char *tmp, t_helper_data *h, int *arr)
{
	if (h->check && h->p > 0)
	{
		tmp[h->p] = '\0';
		if (ft_atoi(tmp) > 255)
		{
			free(tmp);
			return (1);
		}
		if (h->j < 3)
			arr[(h->j)++] = ft_atoi(tmp);
	}
	free(tmp);
	return (0);
}

static int	*get_target_array(t_file_data *file_data, char who_know)
{
	if (who_know == 'F')
		return (file_data->floor_color);
	return (file_data->ceiling_color);
}

int	specific_store(t_file_data *file_data, char who_know, char *buffer)
{
	char			*tmp;
	t_helper_data	*h;
	int				*arr;

	h = malloc(sizeof(t_helper_data));
	if (!h)
		return (1);
	reset_data_helper(h, 1);
	tmp = malloc(4);
	arr = get_target_array(file_data, who_know);
	while (buffer[h->i] && buffer[h->i] != '\n')
	{
		if (parse_step(buffer, &tmp, h, arr))
			return (1);
		h->i++;
	}
    free(h);
	return (finalize_store(tmp, h, arr));
}

int check_if_exact(char **str)
{
    if (str[2])
        return 1;
    else
        return 0;
}










int store_in_the_right_place(char **to_be_splited, t_file_data *file_data, char *buffer)
{
    if (!strcmp(to_be_splited[0], NO))
    {
        if (check_if_exact(to_be_splited))
            return 1;
        file_data->no_texture = dup_trimmed_token(to_be_splited[1]);
        if (!file_data->no_texture)
            return 1;
    }
    else if (!strcmp(to_be_splited[0], SO))
    {
        if (check_if_exact(to_be_splited))
            return 1;
        file_data->so_texture = dup_trimmed_token(to_be_splited[1]);
        if (!file_data->so_texture)
            return 1;
    }
    else if (!strcmp(to_be_splited[0], WE))
    {
        if (check_if_exact(to_be_splited))
            return 1;
        file_data->we_texture = dup_trimmed_token(to_be_splited[1]);
        if (!file_data->we_texture)
            return 1;
    }
    else if (!strcmp(to_be_splited[0], EA))
    {
        if (check_if_exact(to_be_splited))
            return 1;
        file_data->ea_texture = dup_trimmed_token(to_be_splited[1]);
        if (!file_data->ea_texture)
            return 1;
    }
    else if (!strcmp(to_be_splited[0], F))
    {
        if (!comma_length_checker(buffer))
        {
            if (specific_store(file_data, 'F', ft_strchr(buffer, 'F')))
                return 1;
        }
        else
            return (1);
    }
    else if (!strcmp(to_be_splited[0], C))
    {
        if (!comma_length_checker(buffer))
        {
            if (specific_store(file_data, 'C', ft_strchr(buffer, 'C')))
                return (1);
        }
        else
            return (1);
    }
    return (0);
}

int is_empty(char **arry)
{
    int i;

    i = 0;
    while (arry[i])
        i++;
    if (i < 6)
        return (1);
    return (0);
}

int is_color_dup(t_file_data *file_data)
{
    int i;
    int check;

    i = 0;
    check = 0;
    while (file_data->floor_color[i])
    {
        if (file_data->floor_color[i] == file_data->ceiling_color[i])
            check++;
        i++;
    }
    if (check == 3)
        return (1);
    return (0);
}

int is_only_space(char *str)
{
    int i;
    int just_space;

    i = 0;
    just_space = 1;
    if (!str)
    {
        return 1;
    }
    while (str[i])
    {
        if (str[i] != ' ' || str[i] != '\n')
        {
            just_space = 0;
        }
        i++;
    }
    return just_space;
}

int is_betwen(char *line)
{
    int i;

    i = 0;
    while (line[i] && line[i] != '\n')
    {
        if (line[i] != ' ')
            return 1;
        i++;
    }
    // printf("  check  %d\n" , check);
    return 0;
}

void update_line(char *line)
{
    int i;
    int check;
    int is_bet;

    i = 0;
    check = 0;
    is_bet = 0;
    while (line[i])
    {
        if ((line[i] != ' ' && line[i] != '\t'))
            check = 1;
        if (check && (line[i] == ' ' || line[i] == '\t'))
        {
            if (is_betwen(&line[i]))
                line[i] = '1';
        }
        else if (!check && (line[i] == ' ' || line[i] == '\t'))
            line[i] = '+';
        i++;
    }
    // printf("%s" , line);
    // exit(1);
}

int skip_empty_line(char *line)
{
    int i;

    i = 0;
    while (line[i] && line[i] != '\n')
    {
        if (line[i] != ' ' || line[i] != '\t')
            return 0;
        i++;
    }
    return 1;
}
void fill_map(char *line, t_file_data *file_data, int *update_map_arr)
{
    int i;

    i = 0;
    if (skip_empty_line(line) && *update_map_arr == 0)
        return;
    file_data->map[*update_map_arr] = malloc(file_data->element_size);
    while (line[i])
    {
        update_line(line);
        if (line && ft_strlen(line) > file_data->s_element_size)
            file_data->s_element_size = ft_strlen(line);
        file_data->map[*update_map_arr][i] = line[i];
        i++;
    }
    file_data->map[*update_map_arr][i] = '\0';
    (*update_map_arr)++;
}

int already_checked_lenght(char **already_checked)
{
    int i;

    i = 0;
    while (already_checked[i])
        i++;
    return i;
}

int set_data(int fd, t_file_data *file_data)
{
    char *buffer;
    char **to_be_splited;
    t_cmp_data *cmp_data;
    char **already_checked;
    int value;
    int *update_map_arr;
    int fill_only_map;

    cmp_data = malloc(sizeof(t_cmp_data));
    reset_data(cmp_data);
    value = 0;
    update_map_arr = &value;
    fill_only_map = 0;
    already_checked = ft_calloc(7, sizeof(char *));
    file_data->map = ft_calloc(file_data->map_size + 1, sizeof(char *));
    while (1)
    {
        buffer = get_next_line(fd);
        if (buffer)
        {
            to_be_splited = ft_split(buffer, ' ');
            if (!fill_only_map)
            {
                if (!match_in_list(to_be_splited[0], cmp_data->compass,
                                   already_checked))
                {
                    if (store_in_the_right_place(to_be_splited, file_data, buffer))
                    {
                        cleanup_inside_set_data(cmp_data, already_checked, to_be_splited, buffer);
                        return (1);
                    }
                }
                else if (match_in_list(to_be_splited[0], cmp_data->compass,
                                       already_checked) != 0)
                {
                    cleanup_inside_set_data(cmp_data, already_checked, to_be_splited, buffer);
                    return (1);
                }
            }
            if (buffer && !is_empty(already_checked) && fill_only_map)
                fill_map(buffer, file_data, update_map_arr);
            if (!is_empty(already_checked))
                fill_only_map = 1;
            free_double_array(to_be_splited);
            free(buffer);
        }
        else
            break;
    }
    file_data->map[value] = NULL;
    if (is_empty(already_checked))
    {
        free_cmp_data(cmp_data);
        free_double_array(already_checked);
        return (1);
    }
    file_data->s_element_size++;
    free_cmp_data(cmp_data);
    free_double_array(already_checked);
    return (0);
}
