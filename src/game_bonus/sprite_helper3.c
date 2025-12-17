/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:32:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/12/17 18:00:09 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	texture_is_transparent(t_texture *tex, unsigned int color)
{
	unsigned int	masked_color;
	unsigned int	masked_ref;

	masked_color = color & 0x00FFFFFF;
	if (!tex || !tex->addr)
		return (masked_color == 0);
	if (tex->has_transparency)
	{
		masked_ref = tex->transparent_color & 0x00FFFFFF;
		if (masked_color == masked_ref)
			return (true);
	}
	return (masked_color == 0);
}

void	set_frame(t_sprite *sprite)
{
	sprite->frame_w = SPRITE_FRAME_SIZE;
	sprite->frame_h = SPRITE_FRAME_SIZE;
	sprite->frame_cols = sprite->sp_tex->width / sprite->frame_w;
	if (sprite->frame_cols <= 0)
		sprite->frame_cols = 1;
	sprite->frame_rows = sprite->sp_tex->height / sprite->frame_h;
	if (sprite->frame_rows <= 0)
		sprite->frame_rows = 1;
	sprite->frame_count = sprite->frame_cols * sprite->frame_rows;
	if (sprite->frame_count <= 0)
		sprite->frame_count = 1;
}

void	sprite_sheet_init(t_game_data *data, t_sprite *sprite)
{
	if (!sprite)
		return ;
	if (!data->sprite_sheet_loaded)
	{
		data->sprite_sheet.mlx_connection = data->mlx.mlx_connection;
		load_texture(data, &data->sprite_sheet, "textures/spritesheet.xpm");
		if (data->sprite_sheet.width > 0 && data->sprite_sheet.height > 0)
		{
			data->sprite_sheet.transparent_color = texel(&data->sprite_sheet, 0, 0);
			data->sprite_sheet.has_transparency = true;
		}
		data->sprite_sheet_loaded = true;
	}
	sprite->sp_tex = &data->sprite_sheet;
	set_frame(sprite);
}

void	sprite_sheet_destroy(t_game_data *data)
{
	if (!data->sprite_sheet_loaded)
		return ;
	if (data->sprite_sheet.img && data->sprite_sheet.mlx_connection)
	{
		mlx_destroy_image(data->sprite_sheet.mlx_connection, data->sprite_sheet.img);
		data->sprite_sheet.img = NULL;
	}
	data->sprite_sheet.addr = NULL;
	data->sprite_sheet_loaded = false;
	ft_bzero(&data->sprite_sheet, sizeof(data->sprite_sheet));
}

void	append_to_sprite(t_game_data *data, int x, int y)
{
	t_sprite	*sprite;
	t_sprite	*tmp;
	size_t		new_count;

	new_count = data->sprite_count + 1;
	tmp = realloc(data->sprite, sizeof(t_sprite) * new_count);
	if (!tmp)
		return ;
	data->sprite = tmp;
	sprite = &data->sprite[new_count - 1];
	ft_bzero(sprite, sizeof(*sprite));
	sprite->map_x = x;
	sprite->map_y = y;
	sprite->position = vec2(x + 0.5, y + 0.5);
	sprite->frame = 0;
	sprite->frame_time = 0;
	sprite->frame_duration = 0.12;
	sprite->speed = 0.4;
	sprite->velocity = vec2(0, 0);
	sprite->state = SPRITE_IDLE;
	sprite->visible = false;
	sprite_sheet_init(data, sprite);
	data->sprite_count = new_count;
}
