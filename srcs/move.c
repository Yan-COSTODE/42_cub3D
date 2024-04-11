/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:11:09 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 18:39:56 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_up(t_program *program, double move_speed)
{
	int	x;
	int	y;

	program->player.moving = true;
	x = program->player.pos.x + (program->player.dir.x + MOVE_OFFSET
			* program->player.dir.x) * move_speed;
	y = program->player.pos.y + (program->player.dir.y + MOVE_OFFSET
			* program->player.dir.y) * move_speed;
	if (get_at(program->map, x, program->player.pos.y) == '0'
		|| get_at(program->map, x, program->player.pos.y) == (int)(OPEN) + '0')
		program->player.pos.x += program->player.dir.x * move_speed;
	if (get_at(program->map, program->player.pos.x, y) == '0'
		|| get_at(program->map, program->player.pos.x, y) == (int)(OPEN) + '0')
		program->player.pos.y += program->player.dir.y * move_speed;
}

void	move_down(t_program *program, double move_speed)
{
	int	x;
	int	y;

	program->player.moving = true;
	x = program->player.pos.x - (program->player.dir.x + MOVE_OFFSET
			* program->player.dir.x) * move_speed;
	y = program->player.pos.y - (program->player.dir.y + MOVE_OFFSET
			* program->player.dir.y) * move_speed;
	if (get_at(program->map, x, program->player.pos.y) == '0'
		|| get_at(program->map, x, program->player.pos.y) == (int)(OPEN) + '0')
		program->player.pos.x -= program->player.dir.x * move_speed;
	if (get_at(program->map, program->player.pos.x, y) == '0'
		|| get_at(program->map, program->player.pos.x, y) == (int)(OPEN) + '0')
		program->player.pos.y -= program->player.dir.y * move_speed;
}

void	move_right(t_program *program, double move_speed)
{
	int	x;
	int	y;

	program->player.moving = true;
	x = program->player.pos.x - (program->player.dir.y + MOVE_OFFSET
			* program->player.dir.y) * move_speed;
	y = program->player.pos.y + (program->player.dir.x + MOVE_OFFSET
			* program->player.dir.x) * move_speed;
	if (get_at(program->map, x, program->player.pos.y) == '0'
		|| get_at(program->map, x, program->player.pos.y) == (int)(OPEN) + '0')
		program->player.pos.x -= program->player.dir.y * move_speed;
	if (get_at(program->map, program->player.pos.x, y) == '0'
		|| get_at(program->map, program->player.pos.x, y) == (int)(OPEN) + '0')
		program->player.pos.y += program->player.dir.x * move_speed;
}

void	move_left(t_program *program, double move_speed)
{
	int	x;
	int	y;

	program->player.moving = true;
	x = program->player.pos.x + (program->player.dir.y + MOVE_OFFSET
			* program->player.dir.y) * move_speed;
	y = program->player.pos.y - (program->player.dir.x + MOVE_OFFSET
			* program->player.dir.x) * move_speed;
	if (get_at(program->map, x, program->player.pos.y) == '0'
		|| get_at(program->map, x, program->player.pos.y) == (int)(OPEN) + '0')
		program->player.pos.x += program->player.dir.y * move_speed;
	if (get_at(program->map, program->player.pos.x, y) == '0'
		|| get_at(program->map, program->player.pos.x, y) == (int)(OPEN) + '0')
		program->player.pos.y -= program->player.dir.x * move_speed;
}

void	move(t_program *program)
{
	double	move_speed;

	program->player.moving = false;
	if (program->player.sprint)
		move_speed = program->mlx->delta_time * SPRINT_SPEED;
	else if (program->player.crouch)
		move_speed = program->mlx->delta_time * CROUCH_SPEED;
	else
		move_speed = program->mlx->delta_time * MOVE_SPEED;
	if (mlx_is_key_down(program->mlx, MLX_KEY_W))
		move_up(program, move_speed);
	if (mlx_is_key_down(program->mlx, MLX_KEY_S))
		move_down(program, move_speed);
	if (mlx_is_key_down(program->mlx, MLX_KEY_A))
		move_left(program, move_speed);
	if (mlx_is_key_down(program->mlx, MLX_KEY_D))
		move_right(program, move_speed);
	show_shoot_frame(program);
	jump(program);
	crouch(program);
	sprint(program);
}
