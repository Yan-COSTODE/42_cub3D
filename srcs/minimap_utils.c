/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 14:57:46 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 14:58:57 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
			rotate.trans.x = rotate.x - MINIMAP_W / 2;
			rotate.trans.y = rotate.y - MINIMAP_H / 2;
			rotate.rotated.x = rotate.trans.x * rotate.x_rot.x + rotate.trans.y
				* rotate.x_rot.y;
			rotate.rotated.y = rotate.trans.x * rotate.y_rot.x + rotate.trans.y
				* rotate.y_rot.y;
			rotate.rotated.x += MINIMAP_W / 2 + CELL_SIZE * MINIMAP_CELLS;
			rotate.rotated.y += MINIMAP_H / 2 + CELL_SIZE * MINIMAP_CELLS * 1.5;
			rotate.rotated.x = round(rotate.rotated.x);
			rotate.rotated.y = round(rotate.rotated.y);
			rotate_utils(&rotate, program);
		}
	}
}
