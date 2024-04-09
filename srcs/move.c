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

void	rotate_right(t_program *program, double rotate_speed)
{
	double	dir_x;
	double	plane_x;

	dir_x = program->player.dir.x;
	program->player.dir.x = program->player.dir.x * cos(rotate_speed)
		- program->player.dir.y * sin(rotate_speed);
	program->player.dir.y = dir_x * sin(rotate_speed) + program->player.dir.y
		* cos(rotate_speed);
	plane_x = program->player.plane.x;
	program->player.plane.x = program->player.plane.x * cos(rotate_speed)
		- program->player.plane.y * sin(rotate_speed);
	program->player.plane.y = plane_x * sin(rotate_speed)
		+ program->player.plane.y * cos(rotate_speed);
}

void	rotate_left(t_program *program, double rotate_speed)
{
	double	dir_x;
	double	plane_x;

	dir_x = program->player.dir.x;
	program->player.dir.x = program->player.dir.x * cos(-rotate_speed)
		- program->player.dir.y * sin(-rotate_speed);
	program->player.dir.y = dir_x * sin(-rotate_speed) + program->player.dir.y
		* cos(-rotate_speed);
	plane_x = program->player.plane.x;
	program->player.plane.x = program->player.plane.x * cos(-rotate_speed)
		- program->player.plane.y * sin(-rotate_speed);
	program->player.plane.y = plane_x * sin(-rotate_speed)
		+ program->player.plane.y * cos(-rotate_speed);
}

void	rotate_up_down(t_program *program, double diff)
{
	double	pitch;
	double	rotate_speed;

	rotate_speed = program->mlx->delta_time * PITCH_SPEED;
	if (diff > 0)
		pitch = program->player.pitch + rotate_speed;
	else
		pitch = program->player.pitch - rotate_speed;
	if (pitch > PITCH_LIMIT)
		pitch = PITCH_LIMIT;
	else if (pitch < -PITCH_LIMIT)
		pitch = -PITCH_LIMIT;
	program->player.pitch = pitch;
}

void	rotate(t_program *program)
{
	t_coord_int	mouse;
	double		rotate_speed;

	if (program->cursor == MLX_MOUSE_NORMAL)
		return ;
	rotate_speed = program->mlx->delta_time * ROTATE_SPEED;
	if (mlx_is_key_down(program->mlx, MLX_KEY_RIGHT))
		rotate_right(program, rotate_speed);
	if (mlx_is_key_down(program->mlx, MLX_KEY_LEFT))
		rotate_left(program, rotate_speed);
	mlx_get_mouse_pos(program->mlx, &mouse.x, &mouse.y);
	if (mouse.x > WIDTH / 2)
		rotate_right(program, rotate_speed * MOUSE_SENS * ((mouse.x - WIDTH
					/ 2.0) / (WIDTH / 2.0)));
	else if (mouse.x < WIDTH / 2)
		rotate_left(program, rotate_speed * MOUSE_SENS * ((WIDTH / 2.0
					- mouse.x) / (WIDTH / 2.0)));
	if (mouse.y != HEIGHT / 2)
		rotate_up_down(program, HEIGHT / 2 - mouse.y);
	mlx_set_mouse_pos(program->mlx, WIDTH / 2, HEIGHT / 2);
}

void	jump(t_program *program)
{
	if (program->player.jump)
	{
		program->player.height += program->mlx->delta_time * JUMP_FORCE;
		if (program->player.height >= JUMP_MAX)
		{
			program->player.height = JUMP_MAX;
			program->player.jump = false;
			play_land();
		}
	}
	else if (program->player.height > 0)
	{
		program->player.height -= program->mlx->delta_time * GRAVITY_FORCE;
		if (program->player.height <= 0)
			program->player.height = 0;
	}
	if (!mlx_is_key_down(program->mlx, MLX_KEY_SPACE))
		return ;
	if (program->player.height == 0 && !program->player.jump
		&& !program->player.crouch)
	{
		program->player.jump = true;
		play_jump();
	}
}

void	crouch(t_program *program)
{
	if (program->player.crouch)
	{
		program->player.height -= program->mlx->delta_time * CROUCH_FORCE;
		if (program->player.height <= CROUCH_MAX)
			program->player.height = CROUCH_MAX;
	}
	else if (program->player.height < 0)
	{
		program->player.height += program->mlx->delta_time * CROUCH_FORCE;
		if (program->player.height >= 0)
			program->player.height = 0;
	}
	program->player.crouch = mlx_is_key_down(program->mlx,
			MLX_KEY_LEFT_CONTROL);
}

void	sprint(t_program *program)
{
	if (program->player.crouch || program->player.jump)
	{
		program->player.sprint = false;
		return ;
	}
	program->player.sprint = mlx_is_key_down(program->mlx, MLX_KEY_LEFT_SHIFT);
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
