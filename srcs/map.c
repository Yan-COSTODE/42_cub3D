/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:58:51 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/25 17:58:51 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	get_at(const t_map map, int x, int y)
{
	if (!map.content)
		return (' ');
	if ((x > map.width || x < 0) || (y > map.height || y < 0))
		return (' ');
	return (map.content[y][x]);
}

void	set_at(t_map *map, int x, int y, char c)
{
	if (get_at(*map, x, y) == ' ')
		return ;
	map->content[y][x] = c;
}

void	print_map(t_map map)
{
	int x;
	int y;

	y = -1;
	while (++y < map.height)
	{
		x = -1;
		while (++x < map.width)
			print_fd_char(1, get_at(map, x, y));
		print_fd(1, "\n");
	}
}