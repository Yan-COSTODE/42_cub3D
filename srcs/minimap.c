/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:27:56 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/28 20:27:56 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void fill_minimap(t_program *program, t_coord_int x, t_coord_int y, uint32_t color)
{
	t_coord_int index;

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

void fill_display(t_program *program, t_coord_int x, t_coord_int y, uint32_t color)
{
	t_coord_int index;

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

uint32_t bilinearInterp(mlx_image_t* image, double xP, double yP)
{
	t_coord_int x;
	t_coord_int y;

	x.x = (int)(xP);
	x.y = x.x + 1;
	y.x = (int)(yP);
	y.y = y.x + 1;

	t_coord d;
	d.x = xP - x.x;
	d.y = yP - y.y;

	x.x = fmax(0, fmin(x.x, image->width - 1));
	x.y = fmax(0, fmin(x.y, image->width - 1));
	y.x = fmax(0, fmin(y.x, image->height - 1));
	y.y = fmax(0, fmin(y.y, image->height - 1));

	t_color topC;
	topC.r = image->pixels[(image->height * y.x + x.x) * 4] * (1 - d.x) + image->pixels[(image->height * y.x + x.y) * 4] * d.x;
	topC.g = image->pixels[(image->height * y.x + x.x) * 4 + 1] * (1 - d.x) + image->pixels[(image->height * y.x + x.y) * 4 + 1] * d.x;
	topC.b = image->pixels[(image->height * y.x + x.x) * 4 + 2] * (1 - d.x) + image->pixels[(image->height * y.x + x.y) * 4 + 2] * d.x;
	topC.a = image->pixels[(image->height * y.x + x.x) * 4 + 3] * (1 - d.x) + image->pixels[(image->height * y.x + x.y) * 4 + 3] * d.x;
	t_color bottomC;
	bottomC.r = image->pixels[(image->height * y.y + x.x) * 4] * (1 - d.x) + image->pixels[(image->height * y.y + x.y) * 4] * d.x;
	bottomC.g = image->pixels[(image->height * y.y + x.x) * 4 + 1] * (1 - d.x) + image->pixels[(image->height * y.y + x.y) * 4 + 1] * d.x;
	bottomC.b = image->pixels[(image->height * y.y + x.x) * 4 + 2] * (1 - d.x) + image->pixels[(image->height * y.y + x.y) * 4 + 2] * d.x;
	bottomC.a = image->pixels[(image->height * y.y + x.x) * 4 + 3] * (1 - d.x) + image->pixels[(image->height * y.y + x.y) * 4 + 3] * d.x;
	t_color final;
	final.r = topC.r * (1 - d.y) + bottomC.r * d.y;
	final.g = topC.g * (1 - d.y) + bottomC.g * d.y;
	final.b = topC.b * (1 - d.y) + bottomC.b * d.y;
	final.a = topC.a * (1 - d.y) + bottomC.a * d.y;
	return (get_color_rgba(final.r, final.g, final.b, final.a));
}

void rotate_minimap(t_program *program)
{
	double angle = atan2(program->player.dir.y, program->player.dir.x) - M_PI / 2;

	t_coord xRot;
	t_coord yRot;

	xRot.x = cos(angle);
	yRot.x = -sin(angle);
	xRot.y = sin(angle);
	yRot.y = cos(angle);

	int x = -1;
	while (++x < MINIMAP_SIZE * 2)
	{
		int y = -1;
		while (++y < MINIMAP_SIZE)
		{
			double transX = x - MINIMAP_SIZE;
			double transY = y - MINIMAP_SIZE / 2;
			double rotatedX = transX * xRot.x + transY * xRot.y;
			double rotatedY = transX * yRot.x + transY * yRot.y;
			rotatedX += MINIMAP_SIZE;
			rotatedY += MINIMAP_SIZE / 2 + MINIMAP_CELL * 5;
			rotatedX = round(rotatedX);
			rotatedY = round(rotatedY);

			if (0 <= rotatedY && rotatedY < (MINIMAP_SIZE * 2) && 0 <= rotatedX && rotatedX < (MINIMAP_SIZE * 2))
			{
				uint32_t pixel = bilinearInterp(program->minimap.img, rotatedX, rotatedY);
				mlx_put_pixel(program->minimap.display, x, y, pixel);
			}
		}
	}
}

void draw_minimap(t_program *program)
{
	t_coord_int x;
	t_coord_int y;
	t_coord diff;
	t_coord_int player;
	t_coord_int index;
	t_coord_int comp;

	x.x = 0;
	x.y = MINIMAP_SIZE * 2 - 1;
	y.x = 0;
	y.y = MINIMAP_SIZE - 1;
	fill_display(program, x, y, get_color_rgba(47, 53, 66, 255 / 4));
	y.y = MINIMAP_SIZE * 2 - 1;
	fill_minimap(program, x, y, get_color_rgba(47, 53, 66, 255 / 4));
	player.x = (int)(program->player.pos.x);
	diff.x = program->player.pos.x - player.x;
	player.y = (int)(program->map.height - program->player.pos.y);
	diff.y = (program->map.height - program->player.pos.y) - player.y;
	index.x = -1;
	while (++index.x < MINIMAP_CELLS * 2)
	{
		index.y = -1;
		while (++index.y < MINIMAP_CELLS * 2)
		{
			comp.x = player.x - MINIMAP_CELLS + index.x;
			comp.y = player.y - MINIMAP_CELLS + index.y;
			if (comp.x < 0 || comp.x > program->map.width - 1 || comp.y < 0 || comp.y > program->map.height - 1)
				continue;
			x.x = index.x * MINIMAP_CELL - diff.x * MINIMAP_CELL;
			x.y = x.x + MINIMAP_CELL - 1;
			if (x.x < 0)
				x.x = 0;
			y.x = index.y * MINIMAP_CELL - diff.y * MINIMAP_CELL;
			y.y = y.x + MINIMAP_CELL - 1;
			if (y.x < 0)
				y.x = 0;
			if (get_at(program->map, comp.x, comp.y) == '1')
				fill_minimap(program, x, y, get_color_rgba(241, 242, 246, 255));
			else if (get_at(program->map, comp.x, comp.y) != ' ')
				fill_minimap(program, x, y, get_color_rgba(47, 53, 66, 255));
		}
	}
	rotate_minimap(program);
}