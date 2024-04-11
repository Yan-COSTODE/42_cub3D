/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:32:06 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:33:10 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	up_orientation(t_program *program)
{
	if (program->player.orientation == 'N')
	{
		program->player.dir.x = 0;
		program->player.dir.y = -1;
		program->player.plane.x = 1;
		program->player.plane.y = 0;
	}
	else if (program->player.orientation == 'S')
	{
		program->player.dir.x = 0;
		program->player.dir.y = 1;
		program->player.plane.x = -1;
		program->player.plane.y = 0;
	}
}

void	set_player_orientation(t_program *program)
{
	up_orientation(program);
	if (program->player.orientation == 'E')
	{
		program->player.dir.x = 1;
		program->player.dir.y = 0;
		program->player.plane.x = 0;
		program->player.plane.y = 1;
	}
	else if (program->player.orientation == 'W')
	{
		program->player.dir.x = -1;
		program->player.dir.y = 0;
		program->player.plane.x = 0;
		program->player.plane.y = -1;
	}
}

void	parse_player(t_program *program)
{
	t_coord_int	coord;

	coord.y = -1;
	while (++coord.y < program->map.height)
	{
		coord.x = -1;
		while (++coord.x < program->map.width)
		{
			if (get_at(program->map, coord.x, coord.y) == 'N')
				program->player.orientation = 'N';
			if (get_at(program->map, coord.x, coord.y) == 'S')
				program->player.orientation = 'S';
			if (get_at(program->map, coord.x, coord.y) == 'E')
				program->player.orientation = 'E';
			if (get_at(program->map, coord.x, coord.y) == 'W')
				program->player.orientation = 'W';
			if (program->player.orientation != 0)
			{
				program->player.pos.x = coord.x + 0.5;
				program->player.pos.y = coord.y + 0.5;
				set_player_orientation(program);
				return ;
			}
		}
	}
}

void	count_player(t_program *program, t_coord_int *i, int *player)
{
	if (program->map.content[i->y][i->x] == 'N'
		|| program->map.content[i->y][i->x] == 'S'
		|| program->map.content[i->y][i->x] == 'W'
		|| program->map.content[i->y][i->x] == 'E')
	{
		*player = *player + 1;
		set_at(&program->map, i->x, i->y, '0');
	}
}

int	parsing_utils(t_program *program, t_coord_int *i, int *player)
{
	while (program->map.content[i->y])
	{
		i->x = 0;
		while (program->map.content[i->y][i->x])
		{
			if (program->map.content[i->y][i->x] == ' ')
			{
				if (!is_space_surrounded(program, program->map.content, i->y,
						i->x))
				{
					print_error("Space character is not surrounded by 1's");
					return (0);
				}
			}
			i->x++;
			count_player(program, i, player);
		}
		i->y++;
	}
	return (1);
}
