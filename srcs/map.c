/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:58:51 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 18:13:38 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	get_at(const t_map map, int x, int y)
{
	if (!map.content)
		return (' ');
	if ((x >= map.width || x < 0) || (y >= map.height || y < 0))
		return (' ');
	return (map.content[y][x]);
}

void	set_at(t_map *map, int x, int y, char c)
{
	if (get_at(*map, x, y) == ' ')
		return ;
	map->content[y][x] = c;
}
