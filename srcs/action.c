/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:10:59 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:13:40 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
