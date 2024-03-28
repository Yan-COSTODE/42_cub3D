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

uint32_t get_color_image(mlx_image_t* img, int x, int y)
{
	uint32_t coord = (img->height * y + x) * 4;
	return(get_color_rgba(img->pixels[coord], img->pixels[coord + 1], img->pixels[coord + 2], img->pixels[coord + 3]));
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

	double top = get_color_image(image, x.x, y.x) * (1 - d.x) + get_color_image(image, x.y, y.x) * d.x;
	double bottom = get_color_image(image, x.x, y.y) * (1 - d.x) + get_color_image(image, x.y, y.y) * d.x;
	return (top * (1 - d.y) + bottom * d.y);
}

void rotate_minimap(t_program *program)
{
	double angle = atan2(program->player.dir.y, program->player.dir.x);

	t_coord xRot;
	t_coord yRot;

	xRot.x = cos(angle);
	yRot.x = -sin(angle);
	xRot.y = sin(angle);
	yRot.y = cos(angle);

	mlx_image_t* tmp = mlx_new_image(program->mlx, MINIMAP_SIZE * 2, MINIMAP_SIZE);
	int x = -1;
	while (++x < MINIMAP_SIZE * 2)
	{
		int y = -1;
		while (++y < MINIMAP_SIZE)
		{
			double rotatedX = x * xRot.x + y * xRot.y;
			double rotatedY = x * yRot.x + y * yRot.y;

			uint32_t pixel = bilinearInterp(program->minimap.img, rotatedX, rotatedY);
			mlx_put_pixel(tmp, x, y, pixel);
		}
	}
	mlx_delete_image(program->mlx, program->minimap.img);
	program->minimap.img = tmp;
	mlx_image_to_window(program->mlx, program->minimap.img, MINIMAP_OFFSET, MINIMAP_OFFSET);
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
	fill_minimap(program, x, y, get_color_rgba(47, 53, 66, 255 / 4));
	player.x = (int)(program->player.pos.x);
	diff.x = program->player.pos.x - player.x;
	player.y = (int)(program->map.height - program->player.pos.y);
	diff.y = (program->map.height - program->player.pos.y) - player.y;
	index.x = -1;
	while (++index.x < MINIMAP_CELLS * 2)
	{
		index.y = -1;
		while (++index.y < MINIMAP_CELLS)
		{
			comp.x = player.x - MINIMAP_CELLS + index.x;
			comp.y = player.y - MINIMAP_CELLS / 2 + index.y;
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