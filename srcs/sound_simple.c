/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:08:35 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:09:01 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	play_land(void)
{
	system("paplay --volume=49152 sounds/Land.wav &");
}

void	play_jump(void)
{
	system("paplay --volume=49152 sounds/Jump.wav &");
}

void	play_door(void)
{
	system("paplay --volume=65536 sounds/Door.wav &");
}

void	play_door_boom(void)
{
	system("timeout 1s paplay --volume=65536 sounds/Boom.wav &");
}

void	play_gun(void)
{
	system("paplay --volume=32768 sounds/Gun.wav &");
}
