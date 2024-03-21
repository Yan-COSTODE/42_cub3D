#include "cub3d.h"

void fill_map(t_program *program, t_coord_int x, t_coord_int y, uint32_t color)
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
	fill_map(program, x, y, program->map.floor.rgba);
}

void fill_ceiling(t_program *program)
{
	t_coord_int x;
	t_coord_int y;

	x.x = 0;
	x.y = WIDTH - 1;
	y.x = 0;
	y.y = HEIGHT / 2 - 1;
	fill_map(program, x, y, program->map.ceiling.rgba);
}

void draw(t_program *program)
{
	int x;

	x = -1;
	fill_ceiling(program);
	fill_floor(program);
	while (++x < WIDTH)
	{
		t_coord rayDir;
		double cameraX = 2 * x / (double)WIDTH - 1;
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
			if (get_at(program->map, map.x, program->map.height - (map.y + 1)) == '1')
				hit = 1;
		}

		if (side == 0)
			perpWallDist = sideDist.x - deltaDist.x;
		else
			perpWallDist = sideDist.y - deltaDist.y;

		int lineHeight;
		lineHeight = (int)(HEIGHT / perpWallDist);
		int drawStart;
		int drawEnd;
		drawStart = -lineHeight / 2 + HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + HEIGHT / 2;
		if (drawEnd >= HEIGHT)
			drawEnd = HEIGHT - 1;

		mlx_texture_t	*text;
		double wallX;

		if (side == 0)
		{
			wallX = program->player.pos.y + perpWallDist * rayDir.y;
			if (program->player.pos.x < map.x)
				text = program->map.east;
			else
				text = program->map.west;
		}
		else
		{
			wallX = program->player.pos.x + perpWallDist * rayDir.x;
			if (program->player.pos.y < map.y)
				text = program->map.north;
			else
				text = program->map.south;
		}

		wallX -= floor(wallX);
		t_coord_int tex;

		tex.x = (int)(wallX * (double)(text->width));
		if (side == 0 && rayDir.x > 0)
			tex.x = text->width - tex.x - 1;
		else if (side == 1 && rayDir.y < 0)
			tex.x = text->width - tex.x - 1;

		double texStep;
		double texPos;
		int y;

		texStep = 1.0 * text->height / lineHeight;
		texPos = (drawStart - HEIGHT / 2 + lineHeight / 2) * texStep;
		y = drawStart - 1;
		while (++y < drawEnd)
		{
			tex.y = (int)(texPos) & (text->height - 1);
			texPos += texStep;
			uint32_t color = get_color_rgba(text->pixels[(text->height * tex.y + tex.x) * 4], text->pixels[(text->height * tex.y + tex.x) * 4 + 1], text->pixels[(text->height * tex.y + tex.x) * 4 + 2], text->pixels[(text->height * tex.y + tex.x) * 4 + 3]);
			mlx_put_pixel(program->map.img, x, y, color);
		}
	}
}