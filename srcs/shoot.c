/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shoot.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:24 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 20:01:22 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	change_door(t_program *program)
{
	t_raycast	raycast;

	if (program->player.pitch > 150 || program->player.pitch < -150)
		return ;
	raycast.x = WIDTH / 2;
	raycast_1(&raycast, program);
	raycast_2(&raycast, program);
	raycast_3bis(&raycast, program);
	if (get_at(program->map, raycast.map.x, raycast.map.y) == '1')
		return ;
	set_door(program, raycast.map);
}

void	setup_shoot(t_program *program)
{
	int	i;

	i = -1;
	while (++i <= 3)
	{
		if (!program->hud.gun[i])
			continue ;
		mlx_image_to_window(program->mlx, program->hud.gun[i], WIDTH * 0.875
			- program->hud.gun[i]->width, HEIGHT - program->hud.gun[i]->height
			+ MAX_BOBBING);
		program->hud.gun[i]->enabled = false;
	}
}

void	shoot_timer(t_program *program)
{
	if (program->hud.shoot)
	{
		program->timer.gun += program->mlx->delta_time;
		if (program->timer.gun >= ANIM_DELAY)
		{
			++program->hud.gindex;
			if (program->hud.gindex == 1)
			{
				play_gun();
				change_door(program);
			}
			if (program->hud.gindex > 3)
			{
				program->hud.gindex = 0;
				program->hud.shoot = false;
			}
			program->timer.gun = 0;
		}
	}
}

void	show_shoot_frame(t_program *program)
{
	int	i;

	i = -1;
	while (++i <= 3)
	{
		if (!program->hud.gun[i] || i == program->hud.gindex)
			continue ;
		program->hud.gun[i]->enabled = false;
	}
	program->hud.bobbing += program->hud.direction * program->mlx->delta_time;
	if (program->hud.bobbing >= MAX_BOBBING)
	{
		program->hud.bobbing = MAX_BOBBING;
		program->hud.direction *= -1;
	}
	else if (program->hud.bobbing <= 0)
	{
		program->hud.bobbing = 0;
		program->hud.direction *= -1;
	}
	program->hud.gun[program->hud.gindex]->enabled = true;
	program->hud.gun[program->hud.gindex]->instances[0].y = HEIGHT
		- program->hud.gun[program->hud.gindex]->height + program->hud.bobbing;
	shoot_timer(program);
}

void	shoot(t_program *program, bool try)
{
	if (try && !program->hud.shoot)
	{
		program->hud.shoot = true;
		program->timer.gun = 0;
	}
}
