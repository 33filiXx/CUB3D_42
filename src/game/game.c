/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:04:52 by rhafidi           #+#    #+#             */
/*   Updated: 2025/10/17 16:13:36 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static char	*g_example_grid[] = {
    "11111111111111111111",
    "10000000001000000001",
    "10110000101000111101",
    "10100000101000100001",
    "10111110101110101101",
    "10000010000010100001",
    "11110010111010111111",
    "10000010100010000001",
    "10111110101110111101",
    "10100000101000100001",
    "10101111101011101011",
    "10100000001010001001",
    "10111111111010111101",
    "10N00000000000000001",
    "11111111111111111111",
    NULL
};
void	init_example_file_data(t_file_data *file_data)
{
	if (!file_data)
		return ;
	file_data->no_texture = ft_strdup("./textures/wall_north.xpm");
	file_data->so_texture = ft_strdup("./textures/wall_south.xpm");
	file_data->we_texture = ft_strdup("./textures/wall_west.xpm");
	file_data->ea_texture = ft_strdup("./textures/wall_east.xpm");
	file_data->floor_color[0] = 220;
	file_data->floor_color[1] = 100;
	file_data->floor_color[2] = 0;
	file_data->ceiling_color[0] = 150;
	file_data->ceiling_color[1] = 150;
	file_data->ceiling_color[2] = 255;
}

void	init_example_map(t_map *map)
{
    if (!map)
        return ;
    map->grid = g_example_grid;
    map->height = 15;  // Updated height
    map->width = 20;   // Updated width
}

void	init_example_player(t_player *player)
{
    if (!player)
        return ;
    player->pos_x = 13.5;  // Moved player to center area
    player->pos_y = 13.5;
    player->dir_x = -1.0;
    player->dir_y = 0.0;
    player->plane_x = 0.0;
    player->plane_y = 0.66;
    player->move_speed = 0.08;
    player->rot_speed = 0.05;
}

void	init_example_raycast(t_raycast *rc)
{
	if (!rc)
		return ;
	rc->camera_x = 0.0;
	rc->ray_dir_x = -1.0;
	rc->ray_dir_y = 0.0;
	rc->map_x = 3;
	rc->map_y = 3;
	rc->side_dist_x = 0.0;
	rc->side_dist_y = 0.0;
	rc->delta_dist_x = 1.0;
	rc->delta_dist_y = 1.0;
	rc->perp_wall_dist = 1.0;
	rc->step_x = -1;
	rc->step_y = 0;
	rc->hit = 0;
	rc->side = 0;
}

static void	print_example_summary(const t_file_data *file_data,
				const t_map *map,
				const t_player *player,
				const t_raycast *rc)
{
	int	row;

	printf("Textures:\n");
	printf("  NO: %s\n", file_data->no_texture);
	printf("  SO: %s\n", file_data->so_texture);
	printf("  WE: %s\n", file_data->we_texture);
	printf("  EA: %s\n", file_data->ea_texture);
	printf("Floor RGB: %d,%d,%d\n", file_data->floor_color[0],
		file_data->floor_color[1], file_data->floor_color[2]);
	printf("Ceiling RGB: %d,%d,%d\n", file_data->ceiling_color[0],
		file_data->ceiling_color[1], file_data->ceiling_color[2]);
	printf("\nMap (%dx%d):\n", map->width, map->height);
	row = 0;
	while (map->grid[row])
	{
		printf("  %s\n", map->grid[row]);
		row++;
	}
	printf("\nPlayer:\n");
	printf("  pos=(%.2f, %.2f) dir=(%.2f, %.2f) plane=(%.2f, %.2f)\n",
		player->pos_x, player->pos_y, player->dir_x, player->dir_y,
		player->plane_x, player->plane_y);
	printf("  move_speed=%.2f rot_speed=%.2f\n", player->move_speed,
		player->rot_speed);
	printf("\nRaycast sample:\n");
	printf("  camera_x=%.2f ray_dir=(%.2f, %.2f) map=(%d,%d) steps=(%d,%d)\n",
		rc->camera_x, rc->ray_dir_x, rc->ray_dir_y, rc->map_x, rc->map_y,
		rc->step_x, rc->step_y);
	printf("  delta=(%.2f, %.2f) perp_wall_dist=%.2f hit=%d side=%d\n",
		rc->delta_dist_x, rc->delta_dist_y, rc->perp_wall_dist,
		rc->hit, rc->side);
}
// data sample 

void	initiate(t_mlx *mlx, t_game_data *game_data)
{
	mlx->mlx_connection = mlx_init();
	if (!mlx->mlx_connection)
		exit(1);
	mlx->mlx_win = mlx_new_window(mlx->mlx_connection,
			game_data->map.width * TILE, game_data->map.height * TILE, "Cube3D");
	if (!mlx->mlx_win)
	{
		mlx_destroy_display(mlx->mlx_connection);
		free(mlx->mlx_connection);
		exit(1);
	}
	mlx->img = mlx_new_image(mlx->mlx_connection, game_data->map.width * TILE, game_data->map.height * TILE);
	if (!mlx->img)
	{
		mlx_destroy_display(mlx->mlx_connection);
		free(mlx->mlx_connection);
		exit(1);
	}
	mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bits_per_pixel,
			&mlx->line_length, &mlx->endian);
}

void put_pixel(t_mlx *mlx, int x, int y, int color)
{
    int offset = y * mlx->line_length + x * (mlx->bits_per_pixel / 8);
    *(unsigned int*)(mlx->addr + offset) = color;
}  


void calculate_scale_and_offset(t_map *map, int *scale, int *offset_x, int *offset_y)
{
    int max_scale_x = (map->width * TILE) / map->width;
    int max_scale_y = (map->height * TILE) / map->height;
    
    *scale = (max_scale_x < max_scale_y) ? max_scale_x : max_scale_y;
    
    int actual_width = map->width * (*scale);
    int actual_height = map->height * (*scale);
    
    *offset_x = ((map->width * TILE) - actual_width) / 2;
    *offset_y = ((map->height * TILE) - actual_height) / 2;
}
void	draw_player(int i, int j, int color, t_mlx *mlx, int floor_color)
{
	int c_x;
	int c_y;
	int ty = 0;
	int tx = 0;
	int r = 16;
	
	c_x = (j * TILE) + TILE / 2;
	c_y = (i * TILE) + TILE / 2;
	
	while (ty < TILE)
	{
		tx = 0;
		while(tx < TILE)
		{
			if ((pow((j * TILE + tx) - c_x, 2) + pow((i * TILE + ty) - c_y, 2)) <= pow(r, 2))
				put_pixel(mlx, j * TILE + tx, i * TILE + ty, color);
			else
				put_pixel(mlx, j * TILE + tx, i * TILE + ty, floor_color);
			tx++;
		}
		ty++;
	}
}

void	draw_env(t_mlx *mlx, t_map *map, t_file_data *file_data)
{
	int	i;
	int	j;
	int	color;
	int tx;
	int ty;
	int	scale , off_x, off_y;
	int	floor_color, ceiling_color;

	i = 0;
	j = 0;
	calculate_scale_and_offset(map, &scale, &off_x, &off_y);
	floor_color = (file_data->floor_color[0] << 24 
					|file_data->floor_color[1] << 16
					|file_data->floor_color[2] << 8);
	ceiling_color = (file_data->ceiling_color[0] << 24  
				|file_data->ceiling_color[1] << 16
				|file_data->ceiling_color[2] << 8);
	while (i < map->height)
	{
		j = 0;
		while(j < map->width)
		{
			if (map->grid[i][j] == '0')
				color = floor_color;
			else if (map->grid[i][j] == '1')
				color = ceiling_color;
			else if (map->grid[i][j] == 'N')
				color = 0xFF0000;
			else
				color = 0;
			ty = 0;
			tx = 0;
			if (map->grid[i][j] == 'N')
				draw_player(i, j , color, mlx, floor_color);
			else
			{
				while (ty < TILE)
				{
					tx = 0;
					while (tx < TILE)
					{
						put_pixel(mlx, j * TILE + tx, i * TILE + ty, color);
						tx++;
					}
					ty++;
				}
			}
			j++;
		}
		i++;
	}
}


int key_press(int keycode, void *param)
{
    t_game_data *data = (t_game_data *)param;
    
    if (keycode == XK_w)
		move_forward(data);
		else if (keycode == XK_d)
		move_right(data);
    redraw_map(data);
    return (0);
}

int close_window(void *param)
{
    exit(0);
    return (0);
}

int	main(void)
{
	t_game_data game_data;
	
	ft_bzero(&game_data.file_data, sizeof(game_data.file_data));
	ft_bzero(&game_data.map, sizeof(game_data.map));
	ft_bzero(&game_data.player, sizeof(game_data.player));
	ft_bzero(&game_data.rc, sizeof(game_data.rc));
	init_example_file_data(&game_data.file_data);
	init_example_map(&game_data.map);
	init_example_player(&game_data.player);
	init_example_raycast(&game_data.rc);
	print_example_summary(&game_data.file_data, &game_data.map, &game_data.player, &game_data.rc);
	free(game_data.file_data.no_texture);
	free(game_data.file_data.so_texture);
	free(game_data.file_data.we_texture);
	free(game_data.file_data.ea_texture);

    
    initiate(&game_data.mlx, &game_data);
    draw_env(&game_data.mlx, &game_data.map, &game_data.file_data);
	// mlx_key_hook(game_data.mlx.mlx_win, key_press, &game_data);
	mlx_hook(game_data.mlx.mlx_win, DestroyNotify, StructureNotifyMask, close_window, &game_data);
    mlx_put_image_to_window(game_data.mlx.mlx_connection, game_data.mlx.mlx_win, game_data.mlx.img, 0, 0);
    mlx_loop(game_data.mlx.mlx_connection);
	return (0);
}

