/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 13:32:29 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 20:06:09 by ycostode         ###   ########.fr       */
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

void	raycast_1(t_raycast *raycast, t_program *program)
{
	raycast->camera_x = 2 * raycast->x / (double)(WIDTH)-1;
	raycast->ray_dir.x = program->player.dir.x + program->player.plane.x
		* raycast->camera_x;
	raycast->ray_dir.y = program->player.dir.y + program->player.plane.y
		* raycast->camera_x;
	raycast->map.x = (int)(program->player.pos.x);
	raycast->map.y = (int)(program->player.pos.y);
	if (raycast->ray_dir.x == 0)
		raycast->delta_dist.x = BIG;
	else
		raycast->delta_dist.x = fabs(1 / raycast->ray_dir.x);
	if (raycast->ray_dir.y == 0)
		raycast->delta_dist.y = BIG;
	else
		raycast->delta_dist.y = fabs(1 / raycast->ray_dir.y);
}

void	raycast_2(t_raycast *raycast, t_program *program)
{
	if (raycast->ray_dir.x < 0)
	{
		raycast->step.x = -1;
		raycast->side_dist.x = (program->player.pos.x - raycast->map.x)
			* raycast->delta_dist.x;
	}
	else
	{
		raycast->step.x = 1;
		raycast->side_dist.x = (raycast->map.x + 1.0 - program->player.pos.x)
			* raycast->delta_dist.x;
	}
	if (raycast->ray_dir.y < 0)
	{
		raycast->step.y = -1;
		raycast->side_dist.y = (program->player.pos.y - raycast->map.y)
			* raycast->delta_dist.y;
	}
	else
	{
		raycast->step.y = 1;
		raycast->side_dist.y = (raycast->map.y + 1.0 - program->player.pos.y)
			* raycast->delta_dist.y;
	}
}

void	raycast_3(t_raycast *raycast, t_program *program)
{
	raycast->hit = 0;
	raycast->side = 0;
	while (raycast->hit == 0)
	{
		if (raycast->side_dist.x < raycast->side_dist.y)
		{
			raycast->side_dist.x += raycast->delta_dist.x;
			raycast->map.x += raycast->step.x;
			raycast->side = 0;
		}
		else
		{
			raycast->side_dist.y += raycast->delta_dist.y;
			raycast->map.y += raycast->step.y;
			raycast->side = 1;
		}
		if (get_at(program->map, raycast->map.x, raycast->map.y) == '1'
			|| get_at(program->map, raycast->map.x,
				raycast->map.y) == (int)(CLOSED) + '0')
			raycast->hit = 1;
	}
	if (raycast->side == 0)
		raycast->perp_wall_dist = raycast->side_dist.x - raycast->delta_dist.x;
	else
		raycast->perp_wall_dist = raycast->side_dist.y - raycast->delta_dist.y;
}

void	raycast_3bis(t_raycast *raycast, t_program *program)
{
	raycast->hit = 0;
	raycast->side = 0;
	while (raycast->hit == 0)
	{
		if (raycast->side_dist.x < raycast->side_dist.y)
		{
			raycast->side_dist.x += raycast->delta_dist.x;
			raycast->map.x += raycast->step.x;
			raycast->side = 0;
		}
		else
		{
			raycast->side_dist.y += raycast->delta_dist.y;
			raycast->map.y += raycast->step.y;
			raycast->side = 1;
		}
		if (get_at(program->map, raycast->map.x, raycast->map.y) != '0')
			raycast->hit = 1;
	}
	if (raycast->side == 0)
		raycast->perp_wall_dist = raycast->side_dist.x - raycast->delta_dist.x;
	else
		raycast->perp_wall_dist = raycast->side_dist.y - raycast->delta_dist.y;
}

void	raycast_4(t_raycast *raycast, t_program *program)
{
	raycast->line_height = (int)(HEIGHT / raycast->perp_wall_dist);
	raycast->offset = program->player.pitch + (program->player.height
			/ raycast->perp_wall_dist);
	raycast->draw_start = -raycast->line_height / 2 + HEIGHT / 2
		+ raycast->offset;
	if (raycast->draw_start < 0)
		raycast->draw_start = 0;
	raycast->draw_end = raycast->line_height / 2 + HEIGHT / 2 + raycast->offset;
	if (raycast->draw_end >= HEIGHT)
		raycast->draw_end = HEIGHT - 1;
	raycast->text = NULL;
}

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

void	raycast_8(t_raycast *raycast, t_program *program)
{
	uint32_t	color;
	uint32_t	coord;

	raycast->y = raycast->draw_start - 1;
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
