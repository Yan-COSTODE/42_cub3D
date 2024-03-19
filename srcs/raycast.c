#include "cub3d.h"

void fill(t_program *program, t_coord_int x, t_coord_int y, uint32_t color)
{
	t_coord_int index;

	index.x = x.x;
	while (index.x <= x.y)
	{
		index.y = y.x;
		while (index.y <= y.y)
		{
			mlx_put_pixel(program->map.img, index.x, index.y, color);
			++index.y;
		}
		++index.x;
	}
}

void fill_floor(t_program *program)
{
	t_coord_int x;
	t_coord_int y;

	x.x = 0;
	x.y = WIDTH - 1;
	y.x = HEIGHT / 2;
	y.y = HEIGHT - 1;
	fill(program, x, y, program->map.floor.rgba);
}

void fill_ceiling(t_program *program)
{
	t_coord_int x;
	t_coord_int y;

	x.x = 0;
	x.y = WIDTH - 1;
	y.x = 0;
	y.y = HEIGHT / 2 - 1;
	fill(program, x, y, program->map.ceiling.rgba);
}

void draw(t_program *program)
{
	int x;
	int h;
	int w;

	w = WIDTH;
	h = HEIGHT;
	x = -1;
	fill_ceiling(program);
	fill_floor(program);
	while (++x < w)
	{
		t_coord rayDir;
		double cameraX = 2 * x / (double)w - 1;
		rayDir.x = program->player.dir.x + program->player.plane.x * cameraX;
		rayDir.y = program->player.dir.y + program->player.plane.y * cameraX;

		t_coord_int map;
		map.x = (int)program->player.pos.x;
		map.y = (int)program->player.pos.y;

		t_coord sideDist;
		t_coord deltaDist;
		if (rayDir.x == 0)
			deltaDist.x = BIG;
		else
			deltaDist.x = fabs(1 / rayDir.x);
		if (rayDir.y == 0)
			deltaDist.y = BIG;
		else
			deltaDist.y = fabs(1 / rayDir.y);
		double perpWallDist;

		t_coord_int step;
		int hit;
		hit = 0;
		int side;

		if (rayDir.x < 0)
		{
			step.x = -1;
			sideDist.x = (program->player.pos.x - map.x) * deltaDist.x;
		}
		else
		{
			step.x = 1;
			sideDist.x = (map.x + 1.0 - program->player.pos.x) * deltaDist.x;
		}
		if (rayDir.y < 0)
		{
			step.y = -1;
			sideDist.y = (program->player.pos.y - map.y) * deltaDist.y;
		}
		else
		{
			step.y = 1;
			sideDist.y = (map.y + 1.0 - program->player.pos.y) * deltaDist.y;
		}

		while (hit == 0)
		{
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				map.x += step.x;
				side = 0;
			}
			else
			{
				sideDist.y += deltaDist.y;
				map.y += step.y;
				side = 1;
			}
			if (get_at(program->map, map.x, map.y) == '1')
				hit = 1;
		}

		if (side == 0)
			perpWallDist = sideDist.x - deltaDist.x;
		else
			perpWallDist = sideDist.y - deltaDist.y;

		int lineHeight;
		lineHeight = (int)(h / perpWallDist);
		int drawStart;
		int drawEnd;
		drawStart = -lineHeight / 2 + h / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + h / 2;
		if (drawEnd >= h)
			drawEnd = h - 1;

		uint32_t color;
		color = get_color_rgba(255, 107, 129, 255);
		if (side == 1)
			color = get_color_rgba(255 / 2, 107 / 2, 129 / 2, 255);
		for (int l = drawStart; l <= drawEnd; l++)
			mlx_put_pixel(program->map.img, x, l, color);
	}
}

/*
 void raycast(t_program *program)
{
	int x;
	int h;
	int w;

	w = WIDTH;
	h = HEIGHT;
	x = -1;
	fill_ceiling(program);
	fill_floor(program);
	while (++x < w)
	{
		t_coord rayDir;
		double cameraX = 2 * x / (double)w - 1;
		rayDir.x = program->player.dir.x + program->player.plane.x * cameraX;
		rayDir.y = program->player.dir.y + program->player.plane.y * cameraX;

		t_coord_int map;
		map.x = (int)program->player.pos.x;
		map.y = (int)program->player.pos.y;

		t_coord sideDist;
		t_coord deltaDist;
		if (rayDir.x == 0)
			deltaDist.x = BIG;
		else
			deltaDist.x = fabs(1 / rayDir.x);
		if (rayDir.y == 0)
			deltaDist.y = BIG;
		else
			deltaDist.y = fabs(1 / rayDir.y);
		double perpWallDist;

		t_coord_int step;
		int hit;
		hit = 0;
		int side;

		if (rayDir.x < 0)
		{
			step.x = -1;
			sideDist.x = (program->player.pos.x - map.x) * deltaDist.x;
		}
		else
		{
			step.x = 1;
			sideDist.x = (map.x + 1.0 - program->player.pos.x) * deltaDist.x;
		}
		if (rayDir.y < 0)
		{
			step.y = -1;
			sideDist.y = (program->player.pos.y - map.y) * deltaDist.y;
		}
		else
		{
			step.y = 1;
			sideDist.y = (map.y + 1.0 - program->player.pos.y) * deltaDist.y;
		}

		while (hit == 0)
		{
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				map.x += step.x;
				side = 0;
			}
			else
			{
				sideDist.y += deltaDist.y;
				map.y += step.y;
				side = 1;
			}
			if (get_at(program->map, map.x, map.y) == '1')
				hit = 1;
		}

		if (side == 0)
			perpWallDist = sideDist.x - deltaDist.x;
		else
			perpWallDist = sideDist.y - deltaDist.y;

		int lineHeight;
		lineHeight = (int)(h / perpWallDist);
		int drawStart;
		int drawEnd;
		drawStart = -lineHeight / 2 + h / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + h / 2;
		if (drawEnd >= h)
			drawEnd = h - 1;

		uint32_t color;
		color = get_color_rgba(255, 107, 129, 255);
		if (side == 1)
			color = get_color_rgba(255 / 2, 107 / 2, 129 / 2, 255);
		for (int l = drawStart; l <= drawEnd; l++)
			mlx_put_pixel(program->map.img, x, l, color);
	}
}
 */