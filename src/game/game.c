/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:04:52 by rhafidi           #+#    #+#             */
/*   Updated: 2025/10/22 23:23:04 by rhafidi          ###   ########.fr       */
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
    player->pos = vec2_new(2.5, 13.5);
    player->dir = vec2_new(0.0, -1.0);
    player->plane = vec2_new(0.66, 0.0);
    player->move_speed = 0.05;
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
		player->pos.x, player->pos.y, player->dir.x, player->dir.y,
		player->plane.x, player->plane.y);
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

void	draw_plane(t_game_data *data)
{
	int i = 0;
	int curr_x;
	int curr_y;
	double start_x, end_x ,start_y,end_y;
	
	start_x = data->player.pos.x * TILE;
	start_y = data->player.pos.y * TILE;
	end_x = start_x + (data->player.plane.x  * TILE);
	end_y = start_y + (data->player.plane.y  * TILE);
	while(i <= 100)
	{
		double t = (double)(i / 100.0);
		curr_x = start_x + t * (end_x - start_x);
		curr_y = start_y + t * (end_y - start_y);
		put_pixel(&data->mlx, curr_x, curr_y, 0xFF0000);
		i++;
	}
}

void	draw_dir(t_game_data *data)
{
	int i = 0;
	int curr_x;
	int curr_y;
	double start_x, end_x ,start_y,end_y;
	
	start_x = data->player.pos.x * TILE;
	start_y = data->player.pos.y * TILE;
	end_x = start_x + (data->player.dir.x * TILE);
	end_y = start_y + (data->player.dir.y * TILE);
	while(i <= 100)
	{
		double t = (double)(i / 100.0);
		curr_x = start_x + t * (end_x - start_x);
		curr_y = start_y + t * (end_y - start_y);
		put_pixel(&data->mlx, curr_x, curr_y, 0xFF0000);
		i++;
	}
}

void	draw_cam_plane(t_game_data *data)
{
	int i = 0;
	double left_p, right_p;
	double plane_len = 0.65;
	double curr_x, curr_y;

	left_p = data->rc.camera_x - plane_len;
	right_p = data->rc.camera_x + plane_len;
	while (i < 90)
	{
		curr_x = data->player.pos.x + (data->player.dir.x + data->player.plane.x * left_p) * (i / 100.0);
		curr_y = data->player.pos.y + (data->player.dir.y + data->player.plane.y * left_p) * (i / 100.0);
		put_pixel(&data->mlx, curr_x * TILE, curr_y * TILE, 0x00FF00);
		curr_x = data->player.pos.x + (data->player.dir.x + data->player.plane.x * right_p) * (i / 100.0);
		curr_y = data->player.pos.y + (data->player.dir.y + data->player.plane.y * right_p) * (i / 100.0);
		put_pixel(&data->mlx, curr_x * TILE, curr_y * TILE, 0x0000FF);
		i++;
	}
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

void	fill_outer_ppixel(t_game_data *data, int i, int j, int floor_color)
{
	int tx;
	int ty;
	
	ty = i * TILE;
	while (ty < (i + 1) * TILE)
	{
		tx = j * TILE;
		while (tx < (j + 1) * TILE)
		{
			put_pixel(&data->mlx, tx, ty, floor_color);
			tx++;
		}
		ty++;
	}
	// data->map.grid[i][j] = '0';
}

void	draw_player(int color, t_game_data *data)
{
	int c_x;
	int c_y;
	t_mlx *mlx = &data->mlx;
	int ty;
	int tx;
	int r;
	
	c_x = (int)(data->player.pos.x * TILE);
	c_y = (int)(data->player.pos.y * TILE);
	(r = 16, ty = c_y - r);
	while (ty <= c_y + r)
	{
		tx = c_x - r;
		while(tx <= c_x + r)
		{
			if ((pow(tx - c_x, 2) + pow(ty - c_y, 2)) <= pow(r, 2))
			{
				if (tx >= 0 && ty >= 0 && tx < data->map.width * TILE && ty < data->map.height * TILE)
					put_pixel(mlx, tx, ty, color);
			}
			tx++;
		}
		ty++;
	}
}

void	set_colors(t_game_data *data, int *floor_color, int * ceiling_color)
{
	*floor_color = (data->file_data.floor_color[0] << 24 
					|data->file_data.floor_color[1] << 16
					|data->file_data.floor_color[2] << 8);
	*ceiling_color = (data->file_data.ceiling_color[0] << 24  
				|data->file_data.ceiling_color[1] << 16
				|data->file_data.ceiling_color[2] << 8);
}

void	draw_tile(t_game_data *data, int i, int j, int color)
{
	int tx;
	int ty;
	
	ty = 0;
	while (ty < TILE)
	{
		tx = 0;
		while (tx < TILE)
		{
			put_pixel(&data->mlx, j * TILE + tx, i * TILE + ty, color);
			tx++;
		}
		ty++;
	}
}

void	draw_env(t_game_data *data)
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
	calculate_scale_and_offset(&data->map, &scale, &off_x, &off_y);
	set_colors(data, &floor_color, &ceiling_color);
	while (i < data->map.height)
	{
		j = 0;
		while(j < data->map.width)
		{
			if (data->map.grid[i][j] == '0' || data->map.grid[i][j] == 'N' || data->map.grid[i][j] == 'S' || data->map.grid[i][j] == 'E' || data->map.grid[i][j] == 'W')
				color = floor_color;
			else if (data->map.grid[i][j] == '1')
				color = ceiling_color;
			else
				color = 0;
			draw_tile(data, i, j , color);
			j++;
		}
		i++;
	}
	draw_player(0xFF0000, data);
}

void	redraw_map(t_game_data *data)
{
	draw_env(data);
	draw_dir(data);
	draw_plane(data);
	draw_cam_plane(data);
	mlx_put_image_to_window(data->mlx.mlx_connection, data->mlx.mlx_win, data->mlx.img, 0, 0);
}
int	valid_move(t_game_data *data, double new_x, double new_y)
{
	int map_x = (int ) new_x;
	int map_y = (int ) new_y;
	
	if (map_x < 0 || map_x >= data->map.width || map_y < 0 || map_y >= data->map.height)
		return (0);
	if (data->map.grid[map_y][map_x] == '1')
		return(0);
	return (1);
}

void	move_forward(t_game_data *data)
{
	double new_x;
	double new_y;
	
	new_x = data->player.pos.x + (data->player.dir.x * data->player.move_speed);
	new_y = data->player.pos.y + (data->player.dir.y * data->player.move_speed);
	if (valid_move(data, new_x, new_y))
	{
		data->player.pos.x = new_x;
		data->player.pos.y = new_y;
	}
}
void	move_backwards(t_game_data *data)
{
	double new_x;
	double new_y;
	
	new_x = data->player.pos.x - (data->player.dir.x * data->player.move_speed);
	new_y = data->player.pos.y - (data->player.dir.y * data->player.move_speed);
	if (valid_move(data, new_x, new_y))
	{
		data->player.pos.x = new_x;
		data->player.pos.y = new_y;
	}
}

void	rotate_right(t_game_data *data)
{
	t_vec2 old_dir, old_plane;
	
	old_dir.x = data->player.dir.x;
	old_dir.y = data->player.dir.y;
	old_plane.x = data->player.plane.x;
	old_plane.y = data->player.plane.y;
	data->player.dir.x = old_dir.x * cos(data->player.rot_speed) - old_dir.y * sin(data->player.rot_speed);
	data->player.dir.y = old_dir.x * sin(data->player.rot_speed) + old_dir.y * cos(data->player.rot_speed);
	data->player.plane.x = old_plane.x * cos(data->player.rot_speed) - old_plane.y * sin(data->player.rot_speed);
	data->player.plane.y = old_plane.x * sin(data->player.rot_speed) + old_plane.y * cos(data->player.rot_speed);
}

void	rotate_left(t_game_data *data)
{
	t_vec2 old_dir, old_plane;
	
	old_dir.x = data->player.dir.x;
	old_dir.y = data->player.dir.y;
	old_plane.x = data->player.plane.x;
	old_plane.y = data->player.plane.y;
	data->player.dir.x = old_dir.x * cos(-data->player.rot_speed) - old_dir.y * sin(-data->player.rot_speed);
	data->player.dir.y = old_dir.x * sin(-data->player.rot_speed) + old_dir.y * cos(-data->player.rot_speed);
	data->player.plane.x = old_plane.x * cos(-data->player.rot_speed) - old_plane.y * sin(-data->player.rot_speed);
	data->player.plane.y = old_plane.x * sin(-data->player.rot_speed) + old_plane.y * cos(-data->player.rot_speed);
}

int key_press(int keycode, void *param)
{
    t_game_data *data = (t_game_data *)param;
    
    if (keycode == XK_w)
		data->player.moving_forward = 1;
	else if (keycode == XK_s)
		data->player.moving_backward = 1;
	else if (keycode == XK_d)
		data->player.rotating_right = 1;
	else if (keycode == XK_a)
		data->player.rotating_left = 1;
    return (0);
}

int key_release(int keycode, void *param)
{
    t_game_data *data = (t_game_data *)param;
    
    if (keycode == XK_w)
		data->player.moving_forward = 0;
	else if (keycode == XK_s)
		data->player.moving_backward = 0;
	else if (keycode == XK_d)
		data->player.rotating_right = 0;
	else if (keycode == XK_a)
		data->player.rotating_left = 0;
    return (0);
}

int game_loop(void *param)
{
    t_game_data *data = (t_game_data *)param;
    
    if (data->player.moving_forward)
        move_forward(data);
    if (data->player.moving_backward)
        move_backwards(data);
	if (data->player.rotating_right)
		rotate_right(data);
    if (data->player.rotating_left)
		rotate_left(data);
    if (data->player.moving_forward || data->player.moving_backward || data->player.rotating_left || data->player.rotating_right)
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
    draw_env(&game_data);
	draw_dir(&game_data);
	draw_plane(&game_data);
	draw_cam_plane(&game_data);
	mlx_hook(game_data.mlx.mlx_win, KeyPress, KeyPressMask, key_press, &game_data);
	mlx_hook(game_data.mlx.mlx_win, KeyRelease, KeyReleaseMask, key_release, &game_data);
	mlx_loop_hook(game_data.mlx.mlx_connection, game_loop, &game_data);
	mlx_hook(game_data.mlx.mlx_win, DestroyNotify, StructureNotifyMask, close_window, &game_data);
    mlx_put_image_to_window(game_data.mlx.mlx_connection, game_data.mlx.mlx_win, game_data.mlx.img, 0, 0);
    mlx_loop(game_data.mlx.mlx_connection);
	return (0);
}

