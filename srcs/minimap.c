/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:27:56 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 14:48:02 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	fill_minimap(t_program *program, t_coord_int x, t_coord_int y,
		uint32_t color)
{
	t_coord_int	index;

	if (x.x < 0)
		x.x = 0;
	if (x.y >= (int)(program->minimap.img->width))
		x.y = program->minimap.img->width - 1;
	if (y.x < 0)
		y.x = 0;
	if (y.y >= (int)(program->minimap.img->height))
		y.y = program->minimap.img->height - 1;
	index.x = x.x;
	while (index.x <= x.y)
	{
		index.y = y.x;
		while (index.y <= y.y)
		{
			mlx_put_pixel(program->minimap.img, index.x, index.y, color);
			++index.y;
		}
		++index.x;
	}
}

void	fill_display(t_program *program, t_coord_int x, t_coord_int y,
		uint32_t color)
{
	t_coord_int	index;

	if (x.x < 0)
		x.x = 0;
	if (x.y >= (int)(program->minimap.img->width))
		x.y = program->minimap.img->width - 1;
	if (y.x < 0)
		y.x = 0;
	if (y.y >= (int)(program->minimap.img->height))
		y.y = program->minimap.img->height - 1;
	index.x = x.x;
	while (index.x <= x.y)
	{
		index.y = y.x;
		while (index.y <= y.y)
		{
			mlx_put_pixel(program->minimap.display, index.x, index.y, color);
			++index.y;
		}
		++index.x;
	}
}

void	init_draw(t_calcul *calcul, t_program *program)
{
	calcul->x.x = 0;
	calcul->x.y = MINIMAP_SIZE * 2 - 1;
	calcul->y.x = 0;
	calcul->y.y = MINIMAP_SIZE - 1;
	fill_display(program, calcul->x, calcul->y, get_color_rgba(47, 53, 66, 255
			/ 4));
	calcul->x.y = MINIMAP_SIZE * 3 - 1;
	calcul->y.y = MINIMAP_SIZE * 3 - 1;
	fill_minimap(program, calcul->x, calcul->y, get_color_rgba(47, 53, 66, 255
			/ 4));
	calcul->player.x = (int)(program->player.pos.x);
	calcul->diff.x = program->player.pos.x - calcul->player.x;
	calcul->player.y = (int)(program->player.pos.y);
	calcul->diff.y = program->player.pos.y - calcul->player.y;
	calcul->index.x = -MINIMAP_CELLS * 1.5 - 1;
}

void	draw_utils(t_calcul *calcul, t_program *program)
{
	calcul->x.x = ((calcul->index.x - calcul->diff.x + MINIMAP_CELLS * 1.5)
			* MINIMAP_CELL);
	calcul->x.y = calcul->x.x + MINIMAP_CELL - 1;
	calcul->y.x = ((calcul->index.y - calcul->diff.y + MINIMAP_CELLS * 1.5)
			* MINIMAP_CELL);
	calcul->y.y = calcul->y.x + MINIMAP_CELL - 1;
	if (get_at(program->map, calcul->comp.x, calcul->comp.y) == ' ')
		;
	else if (get_at(program->map, calcul->comp.x, calcul->comp.y) == '0'
		|| get_at(program->map, calcul->comp.x, calcul->comp.y) == (int)(OPEN)
		+ '0')
		fill_minimap(program, calcul->x, calcul->y, program->map.floor.rgba);
	else
		fill_minimap(program, calcul->x, calcul->y, get_color_rgba(241, 242,
				246, 255));
}

void	draw_minimap(t_program *program)
{
	t_calcul	calcul;
	const int	max_size = MINIMAP_CELLS * 1.5;

	init_draw(&calcul, program);
	while (++calcul.index.x < max_size)
	{
		calcul.index.y = -MINIMAP_CELLS * 1.5 - 1;
		while (++calcul.index.y < max_size)
		{
			calcul.comp.x = calcul.index.x + calcul.player.x;
			if (calcul.comp.x < 0 || calcul.comp.x >= program->map.width)
				break ;
			calcul.comp.y = calcul.index.y + calcul.player.y;
			if (0 <= calcul.comp.y && calcul.comp.y < program->map.height)
				draw_utils(&calcul, program);
		}
	}
	rotate_minimap(program);
}
