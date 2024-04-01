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

void play_background(t_program *program)
{
	int	status;

	if (waitpid(program->music, &status, WNOHANG) != 0)
	{
		program->music = fork();

		if (program->music == 0)
		{
			execlp("paplay", "paplay", "--volume=49152", "sounds/Background.wav", NULL);
			free(program->filename);
			ft_freesplit(program->map.content);
		}
	}
}

void play_gun()
{
	system("paplay --volume=13107 sounds/Gun.wav &");
}