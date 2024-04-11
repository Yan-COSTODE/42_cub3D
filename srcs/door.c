/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:06:09 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:07:36 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_closing(t_program *program, int i)
{
	program->door.elem[i].status += (1.0 / DOOR_CLOSE)
		* program->mlx->delta_time;
	if (program->door.elem[i].status >= CLOSING + 1)
	{
		program->door.elem[i].status = CLOSED;
		set_at(&program->map, program->door.elem[i].pos.x,
			program->door.elem[i].pos.y, (int)(CLOSED) + '0');
		play_door_boom();
	}
}

void	update_opening(t_program *program, int i)
{
	program->door.elem[i].status += (1.0 / DOOR_OPEN)
		* program->mlx->delta_time;
	if (program->door.elem[i].status >= OPENING + 1)
	{
		program->door.elem[i].status = OPEN;
		set_at(&program->map, program->door.elem[i].pos.x,
			program->door.elem[i].pos.y, (int)(OPEN) + '0');
		play_door_boom();
	}
}

void	update_door(t_program *program)
{
	int	i;

	i = -1;
	while (++i < program->door.len)
	{
		if (program->door.elem[i].status >= CLOSING
			&& program->door.elem[i].status < CLOSING + 1)
			update_closing(program, i);
		else if (program->door.elem[i].status >= OPENING
			&& program->door.elem[i].status < OPENING + 1)
			update_opening(program, i);
	}
}

double	get_door(t_program *program, t_coord_int coord)
{
	int	i;

	i = -1;
	while (++i < program->door.len)
	{
		if (program->door.elem[i].pos.x == coord.x
			&& program->door.elem[i].pos.y == coord.y)
		{
			if (program->door.elem[i].status >= CLOSING
				&& program->door.elem[i].status < CLOSING + 1)
				return (program->door.elem[i].status - CLOSING);
			else if (program->door.elem[i].status >= OPENING
				&& program->door.elem[i].status < OPENING + 1)
				return ((OPENING + 1) - program->door.elem[i].status);
			else if (program->door.elem[i].status == OPEN)
				return (0.0);
			else if (program->door.elem[i].status == CLOSED)
				return (1.0);
		}
	}
	return (0.0);
}

void	set_door(t_program *program, t_coord_int coord)
{
	int	i;

	i = -1;
	while (++i < program->door.len)
	{
		if (program->door.elem[i].pos.x == coord.x
			&& program->door.elem[i].pos.y == coord.y)
		{
			if (program->door.elem[i].status == CLOSED)
			{
				program->door.elem[i].status = OPENING;
				set_at(&program->map, program->door.elem[i].pos.x,
					program->door.elem[i].pos.y, (int)(OPENING) + '0');
				play_door();
			}
			else if (program->door.elem[i].status == OPEN)
			{
				program->door.elem[i].status = CLOSING;
				set_at(&program->map, program->door.elem[i].pos.x,
					program->door.elem[i].pos.y, (int)(CLOSING) + '0');
				play_door();
			}
		}
	}
}
