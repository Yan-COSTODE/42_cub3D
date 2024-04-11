/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:22:25 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:22:48 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	raycast_5(t_raycast *raycast, t_program *program)
{
	if (raycast->side == 0)
	{
		raycast->wall_x = program->player.pos.y + raycast->perp_wall_dist
			* raycast->ray_dir.y;
		if (get_at(program->map, raycast->map.x,
				raycast->map.y) == (int)(CLOSED) + '0')
			raycast->text = program->door.img;
		else if (program->player.pos.x < raycast->map.x)
			raycast->text = program->map.west;
		else
			raycast->text = program->map.east;
	}
	else
	{
		raycast->wall_x = program->player.pos.x + raycast->perp_wall_dist
			* raycast->ray_dir.x;
		if (get_at(program->map, raycast->map.x,
				raycast->map.y) == (int)(CLOSED) + '0')
			raycast->text = program->door.img;
		else if (program->player.pos.y < raycast->map.y)
			raycast->text = program->map.south;
		else
			raycast->text = program->map.north;
	}
}

void	raycast_5bis(t_raycast *raycast, t_program *program)
{
	if (raycast->side == 0)
	{
		raycast->wall_x = program->player.pos.y + raycast->perp_wall_dist
			* raycast->ray_dir.y;
		if (get_at(program->map, raycast->map.x, raycast->map.y) != '1'
			&& get_at(program->map, raycast->map.x, raycast->map.y) != ' ')
			raycast->text = program->door.img;
	}
	else
	{
		raycast->wall_x = program->player.pos.x + raycast->perp_wall_dist
			* raycast->ray_dir.x;
		if (get_at(program->map, raycast->map.x, raycast->map.y) != '1'
			&& get_at(program->map, raycast->map.x, raycast->map.y) != ' ')
			raycast->text = program->door.img;
	}
}

void	raycast_6(t_raycast *raycast, t_program *program)
{
	raycast->wall_x -= floor(raycast->wall_x);
	raycast->tex.x = (int)(raycast->wall_x * (double)(MAX_RES));
	if (raycast->side == 0 && raycast->ray_dir.x > 0)
		raycast->tex.x = MAX_RES - raycast->tex.x - 1;
	else if (raycast->side == 1 && raycast->ray_dir.y < 0)
		raycast->tex.x = MAX_RES - raycast->tex.x - 1;
	raycast->tex_step = 1.0 * MAX_RES / raycast->line_height;
	raycast->tex_pos = (raycast->draw_start - program->player.pitch
			- (program->player.height / raycast->perp_wall_dist) - HEIGHT / 2
			+ raycast->line_height / 2) * raycast->tex_step;
	if (raycast->draw_end < 0)
		raycast->draw_end = HEIGHT;
}

void	raycast_7(t_raycast *raycast, t_program *program)
{
	uint32_t	color;

	raycast->y = -1;
	while (++raycast->y <= raycast->draw_start)
	{
		raycast->current_dist = (HEIGHT - (2.0 * program->player.height))
			/ (HEIGHT - 2.0 * (raycast->y - program->player.pitch));
		color = set_fog(program->map.ceiling.rgba, raycast->current_dist
				/ FOG_LENGTH);
		mlx_put_pixel(program->map.img, raycast->x, raycast->y, color);
	}
	raycast->y = raycast->draw_end - 1;
	while (++raycast->y < HEIGHT)
	{
		raycast->current_dist = (HEIGHT + (2.0 * program->player.height)) / (2.0
				* (raycast->y - program->player.pitch) - HEIGHT);
		color = set_fog(program->map.floor.rgba, raycast->current_dist
				/ FOG_LENGTH);
		mlx_put_pixel(program->map.img, raycast->x, raycast->y, color);
	}
}

void	raycast_7bis(t_raycast *raycast, t_program *program)
{
	uint32_t	coord;
	uint32_t	color;

	raycast->y = raycast->draw_end - (raycast->draw_end - raycast->draw_start)
		* get_door(program, raycast->map) - 1;
	while (++raycast->y < raycast->draw_end)
	{
		raycast->tex.y = (int)(raycast->tex_pos) & (MAX_RES - 1);
		raycast->tex_pos += raycast->tex_step;
		coord = (MAX_RES * raycast->tex.y + (MAX_RES - raycast->tex.x)) * 4;
		color = get_color_rgba(raycast->text->pixels[coord],
				raycast->text->pixels[coord + 1], raycast->text->pixels[coord
				+ 2], raycast->text->pixels[coord + 3]);
		color = set_fog(color, raycast->perp_wall_dist / (FOG_LENGTH * 1.15));
		mlx_put_pixel(program->map.img, raycast->x, raycast->y, color);
	}
}
