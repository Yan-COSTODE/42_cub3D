/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:32:06 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:33:05 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_door(t_program *program, t_coord_int coord)
{
	program->door.elem = ft_calloc(1, sizeof(t_door_elem));
	program->door.elem[0].status = CLOSED;
	program->door.elem[0].pos = coord;
	set_at(&program->map, coord.x, coord.y, (int)(CLOSED) + '0');
	++program->door.len;
}

void	add_door(t_program *program, t_coord_int coord)
{
	t_door_elem	*tmp;
	int			i;

	if (program->door.len == 0)
		return (init_door(program, coord));
	tmp = ft_calloc(program->door.len, sizeof(t_door_elem));
	i = -1;
	while (++i < program->door.len)
		tmp[i] = program->door.elem[i];
	free(program->door.elem);
	program->door.elem = ft_calloc(program->door.len + 1, sizeof(t_door_elem));
	i = -1;
	while (++i < program->door.len)
		program->door.elem[i] = tmp[i];
	free(tmp);
	program->door.elem[i].status = CLOSED;
	program->door.elem[i].pos = coord;
	set_at(&program->map, coord.x, coord.y, (int)(CLOSED) + '0');
	++program->door.len;
}

void	door_horizontal(t_program *program, t_coord_int *coord, char **map)
{
	coord->x -= 1;
	if (coord->x >= 0 && map[coord->y][coord->x] == '1' && coord->x - 1 >= 0
		&& (map[coord->y][coord->x - 1] == '.' || map[coord->y][coord->x
			- 1] == '0'))
		add_door(program, *coord);
	coord->x += 2;
	if (coord->x < program->map.width && map[coord->y][coord->x] == '1'
		&& coord->x + 1 < program->map.width && (map[coord->y][coord->x
			+ 1] == '.' || map[coord->y][coord->x + 1] == '0'))
	{
		add_door(program, *coord);
	}
}

void	door_vertical(t_program *program, t_coord_int *coord, char **map)
{
	coord->x -= 1;
	coord->y -= 1;
	if (coord->y >= 0 && map[coord->y][coord->x] == '1' && coord->y - 1 >= 0
		&& (map[coord->y - 1][coord->x] == '.' || map[coord->y
			- 1][coord->x] == '0'))
		add_door(program, *coord);
	coord->y += 2;
	if (coord->y < program->map.height && map[coord->y][coord->x] == '1'
		&& coord->y + 1 < program->map.height && (map[coord->y
			+ 1][coord->x] == '.' || map[coord->y + 1][coord->x] == '0'))
		add_door(program, *coord);
}

void	parse_door(t_program *program)
{
	char		**map;
	t_coord_int	coord;

	map = ft_strdup_split(program->map.content);
	coord.x = -1;
	while (++coord.x < program->map.width)
	{
		coord.y = -1;
		while (++coord.y < program->map.height)
		{
			if (map[coord.y][coord.x] == '0')
			{
				flood_fill(program, map, coord.x, coord.y);
				door_horizontal(program, &coord, map);
				door_vertical(program, &coord, map);
			}
		}
	}
	ft_freesplit(map);
}
