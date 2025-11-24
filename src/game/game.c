/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:04:52 by rhafidi           #+#    #+#             */
/*   Updated: 2025/11/24 21:10:16 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"


void	set_colors(t_game_data *data, unsigned int *floor_color, unsigned int * ceiling_color);

static char	*g_example_grid[] = {
    "11111111111111111111",
    "10000000001000000001",
    "10110000101000111101",
    "10100000D01000100001",
    "10111110101110101101",
    "10000010000010100001",
    "111100101D1010111111",
    "10000010100010000001",
    "10111110101110111101",
    "10100000101000100001",
    "1S1011D1101011101011",
    "101D0000001010001001",
    "1S111111111010111101",
    "10N000S00S0000000001",
    "11111111111111111111",
    NULL
};
void	init_example_file_data(t_file_data *file_data)
{
	if (!file_data)
		return ;
	file_data->no_texture = ft_strdup("../../textures/wall_north.xpm");
	file_data->so_texture = ft_strdup("../../textures/wall_south.xpm");
	file_data->we_texture = ft_strdup("../../textures/wall_west.xpm");
	file_data->ea_texture = ft_strdup("../../textures/wall_east.xpm");
	printf("init textures ptrs: NO=%p SO=%p WE=%p EA=%p\n",
		(void *)file_data->no_texture, (void *)file_data->so_texture,
		(void *)file_data->we_texture, (void *)file_data->ea_texture);
	file_data->floor_color[0] = 150;
	file_data->floor_color[1] = 75;
	file_data->floor_color[2] = 60;
	file_data->ceiling_color[0] = 135;
	file_data->ceiling_color[1] = 206;
	file_data->ceiling_color[2] = 250;
}

void	init_doors(t_game_data *data)
{
	data->door_count = 0;
	data->doors = NULL;
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
    player->move_speed = 0.02;
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
	printf("framebuffer bpp=%d endian=%d line_len=%d\n",
		mlx->bits_per_pixel, mlx->endian, mlx->line_length);
}

// init mlx

// void	draw_plane(t_game_data *data)
// {
// 	int i = 0;
// 	int curr_x;
// 	int curr_y;
// 	double start_x, end_x ,start_y,end_y;
	
// 	start_x = data->player.pos.x * TILE;
// 	start_y = data->player.pos.y * TILE;
// 	end_x = start_x + (data->player.plane.x  * TILE);
// 	end_y = start_y + (data->player.plane.y  * TILE);
// 	while(i <= 100)
// 	{
// 		double t = (double)(i / 100.0);
// 		curr_x = start_x + t * (end_x - start_x);
// 		curr_y = start_y + t * (end_y - start_y);
// 		put_pixel(&data->mlx, curr_x, curr_y, 0xFF0000);
// 		i++;
// 	}
// }

// void	draw_dir(t_game_data *data)
// {
// 	int i = 0;
// 	int curr_x;
// 	int curr_y;
// 	double start_x, end_x ,start_y,end_y;
	
// 	start_x = data->player.pos.x * TILE;
// 	start_y = data->player.pos.y * TILE;
// 	end_x = start_x + (data->player.dir.x * TILE);
// 	end_y = start_y + (data->player.dir.y * TILE);
// 	while(i <= 100)
// 	{
// 		double t = (double)(i / 100.0);
// 		curr_x = start_x + t * (end_x - start_x);
// 		curr_y = start_y + t * (end_y - start_y);
// 		put_pixel(&data->mlx, curr_x, curr_y, 0xFF0000);
// 		i++;
// 	}
// }

// void	draw_cam_plane(t_game_data *data)
// {
// 	int i = 0;
// 	double left_p, right_p;
// 	double plane_len = 0.65;
// 	double curr_x, curr_y;

// 	left_p = data->rc.camera_x - plane_len;
// 	right_p = data->rc.camera_x + plane_len;
// 	while (i < 90)
// 	{
// 		curr_x = data->player.pos.x + (data->player.dir.x + data->player.plane.x * left_p) * (i / 100.0);
// 		curr_y = data->player.pos.y + (data->player.dir.y + data->player.plane.y * left_p) * (i / 100.0);
// 		put_pixel(&data->mlx, curr_x * TILE, curr_y * TILE, 0x00FF00);
// 		curr_x = data->player.pos.x + (data->player.dir.x + data->player.plane.x * right_p) * (i / 100.0);
// 		curr_y = data->player.pos.y + (data->player.dir.y + data->player.plane.y * right_p) * (i / 100.0);
// 		put_pixel(&data->mlx, curr_x * TILE, curr_y * TILE, 0x0000FF);
// 		i++;
// 	}
// }


// all of the above code is for the minimap some funcs arent used anymore !!

void	redraw_map(t_game_data *data)
{
	int map_width;

	map_width = data->map.width * TILE;
	render_3d_view(data, 0, map_width, data->map.height * TILE);
	sprite_render_all(data, 0, map_width, data->map.height * TILE);
	draw_env(data);
    
    mlx_put_image_to_window(data->mlx.mlx_connection, data->mlx.mlx_win, data->mlx.img, 0, 0);
}


int game_loop(void *param)
{
    t_game_data *data = (t_game_data *)param;
	double now = get_now_seconds();
	double dt = now - data->last_time;
	bool moved = false;
	data->last_time = now;
	door_update(data, dt);
	sprite_update_all(data, dt);
    set_moved_flag(data, &moved);
	if (moved || data->sprite_count > 0)
		redraw_map(data);
    return (0);
}

int close_window(void *param)
{
	t_game_data *data = (t_game_data *)param;
	cleanup_game(data);
    exit(0);
    return (0);
}

// all of above code is for movement && processing events

int	main(void)
{
	t_game_data game_data;
	

	// insert ur parsing in this part
	ft_bzero(&game_data, sizeof(game_data));
	ft_bzero(&game_data.file_data, sizeof(game_data.file_data));
	ft_bzero(&game_data.map, sizeof(game_data.map));
	ft_bzero(&game_data.player, sizeof(game_data.player));
	ft_bzero(&game_data.rc, sizeof(game_data.rc));
	init_example_file_data(&game_data.file_data);
	init_example_map(&game_data.map);
	init_example_player(&game_data.player);
	init_example_raycast(&game_data.rc);
	init_doors(&game_data);
	game_data.last_time = get_now_seconds();
	print_example_summary(&game_data.file_data, &game_data.map, &game_data.player, &game_data.rc);
	// the t_game_data struct should be filled properly as the data sample above shows awalid dzb
    // my raycasting and bonus componenets
	
	initiate(&game_data.mlx, &game_data);
	door_load_map(&game_data);
	sprite_load_map(&game_data);
	sprite_update_all(&game_data, 0.0);
	redraw_map(&game_data);
	mlx_hook(game_data.mlx.mlx_win, KeyPress, KeyPressMask, key_press, &game_data);
	mlx_hook(game_data.mlx.mlx_win, KeyRelease, KeyReleaseMask, key_release, &game_data);
	mlx_loop_hook(game_data.mlx.mlx_connection, game_loop, &game_data);
	mlx_hook(game_data.mlx.mlx_win, DestroyNotify, StructureNotifyMask, close_window, &game_data);
    mlx_put_image_to_window(game_data.mlx.mlx_connection, game_data.mlx.mlx_win, game_data.mlx.img, 0, 0);
	init_mouse(&game_data);
	mlx_hook(game_data.mlx.mlx_win, MotionNotify, PointerMotionMask, on_mouse_move, &game_data);
    mlx_loop(game_data.mlx.mlx_connection);
	cleanup_game(&game_data);
	return (0);
}

