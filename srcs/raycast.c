/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 13:32:29 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/12 16:49:36 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

uint32_t	set_fog(uint32_t original, double scale)
{
	t_color	fog_color;
	t_color	color;
	double	elem;

	if (scale > MAX_FOG)
		scale = MAX_FOG;
	else if (scale < 0)
		scale = 0;
	fog_color.r = 0;
	fog_color.g = 0;
	fog_color.b = 0;
	elem = (original & RED_MASK) >> 24;
	color.r = fog_color.r * scale + elem * (1.0 - scale);
	elem = (original & GREEN_MASK) >> 16;
	color.g = fog_color.g * scale + elem * (1.0 - scale);
	elem = (original & BLUE_MASK) >> 8;
	color.b = fog_color.b * scale + elem * (1.0 - scale);
	return (get_color_rgba(color.r, color.g, color.b, 255));
}

void	raycast_8(t_raycast *raycast, t_program *program)
{
	uint32_t	color;
	uint32_t	coord;

	raycast->y = raycast->draw_start - 1;
	++raycast->tex.x;
	while (++raycast->y < raycast->draw_end)
	{
		raycast->tex.y = (int)(raycast->tex_pos) & (MAX_RES - 1);
		raycast->tex_pos += raycast->tex_step;
		coord = (raycast->text->width * raycast->tex.y + (raycast->text->width
					- raycast->tex.x)) * 4;
		color = get_color_rgba(raycast->text->pixels[coord],
				raycast->text->pixels[coord + 1], raycast->text->pixels[coord
				+ 2], raycast->text->pixels[coord + 3]);
		color = set_fog(color, raycast->perp_wall_dist / (FOG_LENGTH * 1.15));
		mlx_put_pixel(program->map.img, raycast->x, raycast->y, color);
	}
}

void	draw_wall(t_program *program)
{
	t_raycast	raycast;

	raycast.x = -1;
	while (++raycast.x < WIDTH)
	{
		raycast_1(&raycast, program);
		raycast_2(&raycast, program);
		raycast_3(&raycast, program);
		raycast_4(&raycast, program);
		raycast_5(&raycast, program);
		raycast_6(&raycast, program);
		raycast_7(&raycast, program);
		raycast_8(&raycast, program);
	}
}

void	draw_door(t_program *program)
{
	t_raycast	raycast;

	raycast.x = -1;
	while (++raycast.x < WIDTH)
	{
		raycast_1(&raycast, program);
		raycast_2(&raycast, program);
		raycast_3bis(&raycast, program);
		if (get_at(program->map, raycast.map.x, raycast.map.y) == '1'
			|| get_at(program->map, raycast.map.x, raycast.map.y) == ' ')
			continue ;
		raycast_4(&raycast, program);
		raycast_5bis(&raycast, program);
		raycast_6(&raycast, program);
		if (!raycast.text)
			continue ;
		raycast_7bis(&raycast, program);
	}
}

void	draw(t_program *program)
{
	draw_wall(program);
	draw_door(program);
}
