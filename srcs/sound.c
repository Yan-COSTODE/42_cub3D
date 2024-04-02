/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 12:47:43 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/01 13:48:50 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void play_land()
{
	system("paplay --volume=49152 sounds/Land.wav &");
}

void play_jump()
{
	system("paplay --volume=49152 sounds/Jump.wav &");
}

void play_walk(t_program *program)
{
	if (!program->player.moving && program->timer.footstep == 0)
		return ;
	if (program->player.moving && program->timer.footstep == 0)
		system("timeout 0.2s paplay --volume=65536 sounds/Walk.wav &");
	program->timer.footstep += program->mlx->delta_time;
	if (program->timer.footstep >= 0.2)
		program->timer.footstep = 0;
}

void play_breath(t_program *program)
{
	if (program->timer.breath == 0)
		system("timeout 2.5s paplay --volume=49152 sounds/Breathing.wav &");
	program->timer.breath += program->mlx->delta_time;
	if (program->timer.breath >= 2.95)
		program->timer.breath = 0;
}

void play_background(t_program *program)
{
	if (program->timer.music == 0)
		system("paplay --volume=32768 sounds/Background.wav &");
	program->timer.music += program->mlx->delta_time;
	if (program->timer.music >= MUSIC_LENGTH)
		program->timer.music = 0;
}

void play_gun()
{
	system("paplay --volume=32768 sounds/Gun.wav &");
}

void update_sound(t_program *program)
{
	play_background(program);
	play_walk(program);
	play_breath(program);
}