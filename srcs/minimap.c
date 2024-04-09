/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:27:56 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 19:54:51 by ycostode         ###   ########.fr       */
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

uint32_t	get_pixel(mlx_image_t *image, double xP, double yP)
{
	t_coord_int	coord;
	t_color		test;

	coord.x = (int)(xP);
	coord.y = (int)(yP);
	coord.x = fmax(0, fmin(coord.x, image->width - 1));
	coord.y = fmax(0, fmin(coord.y, image->height - 1));
	test.r = image->pixels[(image->height * coord.y + coord.x) * 4];
	test.g = image->pixels[(image->height * coord.y + coord.x) * 4 + 1];
	test.b = image->pixels[(image->height * coord.y + coord.x) * 4 + 2];
	test.a = image->pixels[(image->height * coord.y + coord.x) * 4 + 3];
	return (get_color_rgba(test.r, test.g, test.b, test.a));
}

void	init_rotate(t_rotate *rotate, t_program *program)
{
	rotate->angle = atan2(program->player.dir.y, -program->player.dir.x) + M_PI
		/ 2;
	rotate->x_rot.x = cos(rotate->angle);
	rotate->y_rot.x = -sin(rotate->angle);
	rotate->x_rot.y = sin(rotate->angle);
	rotate->y_rot.y = cos(rotate->angle);
	rotate->x = -1;
}

void	rotate_utils(t_rotate *rotate, t_program *program)
{
	if (0 <= rotate->rotated.y
		&& rotate->rotated.y < program->minimap.img->height
		&& 0 <= rotate->rotated.x
		&& rotate->rotated.x < program->minimap.img->width)
	{
		rotate->pixel = get_pixel(program->minimap.img, rotate->rotated.x,
				rotate->rotated.y);
		mlx_put_pixel(program->minimap.display, rotate->x, rotate->y,
			rotate->pixel);
	}
}

void	rotate_minimap(t_program *program)
{
	t_rotate	rotate;

	init_rotate(&rotate, program);
	while (++rotate.x < (int)(program->minimap.display->width))
	{
		rotate.y = -1;
		while (++rotate.y < (int)(program->minimap.display->height))
		{
			rotate.trans.x = rotate.x - MINIMAP_SIZE;
			rotate.trans.y = rotate.y - MINIMAP_SIZE / 2;
			rotate.rotated.x = rotate.trans.x * rotate.x_rot.x + rotate.trans.y
				* rotate.x_rot.y;
			rotate.rotated.y = rotate.trans.x * rotate.y_rot.x + rotate.trans.y
				* rotate.y_rot.y;
			rotate.rotated.x += MINIMAP_SIZE + MINIMAP_CELL * MINIMAP_CELLS / 2;
			rotate.rotated.y += MINIMAP_SIZE / 2 + MINIMAP_CELL * MINIMAP_CELLS;
			rotate.rotated.x = round(rotate.rotated.x);
			rotate.rotated.y = round(rotate.rotated.y);
			rotate_utils(&rotate, program);
		}
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
	calcul->player.y = (int)(program->map.height - program->player.pos.y);
	calcul->diff.y = program->player.pos.y - calcul->player.y;
	calcul->index.x = -1;
}

void	draw_utils(t_calcul *calcul, t_program *program)
{
	calcul->x.x = calcul->index.x * MINIMAP_CELL - calcul->diff.x
		* MINIMAP_CELL;
	calcul->x.y = calcul->x.x + MINIMAP_CELL - 1;
	if (calcul->x.x < 0)
		calcul->x.x = 0;
	calcul->y.x = calcul->index.y * MINIMAP_CELL - calcul->diff.y
		* MINIMAP_CELL;
	calcul->y.y = calcul->y.x + MINIMAP_CELL - 1;
	if (calcul->y.x < 0)
		calcul->y.x = 0;
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

	init_draw(&calcul, program);
	while (++calcul.index.x < MINIMAP_CELLS * 3)
	{
		calcul.index.y = -1;
		while (++calcul.index.y < MINIMAP_CELLS * 3)
		{
			calcul.comp.x = calcul.player.x - MINIMAP_CELLS * 1.5
				+ calcul.index.x;
			calcul.comp.y = calcul.player.y - MINIMAP_CELLS * 1.5
				+ calcul.index.y;
			if (0 <= calcul.comp.x && calcul.comp.x < program->map.width
				&& 0 <= calcul.comp.y && calcul.comp.y < program->map.height)
				draw_utils(&calcul, program);
		}
	}
	rotate_minimap(program);
}
