/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:10:39 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:13:49 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	double	rotate_speed;

	diff /= HEIGHT / 2;
	rotate_speed = program->mlx->delta_time * PITCH_SPEED;
	program->player.pitch += rotate_speed * MOUSE_SENS * diff;
	if (program->player.pitch > PITCH_LIMIT)
		program->player.pitch = PITCH_LIMIT;
	else if (program->player.pitch < -PITCH_LIMIT)
		program->player.pitch = -PITCH_LIMIT;
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
