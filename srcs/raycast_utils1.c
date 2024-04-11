/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:20:22 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:22:45 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
