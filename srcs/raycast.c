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

void draw_background_no_cast(t_program *program)
{
	(void)program;
}

void draw_background(t_program *program)
{
	int y;

	y = -1;
	while (++y < HEIGHT)
	{
		bool is_floor = y > HEIGHT / 2 + program->player.pitch;
		t_coord rayDirX;
		t_coord rayDirY;
		rayDirX.x = program->player.dir.x - program->player.plane.x;
		rayDirX.y = program->player.dir.x + program->player.plane.x;
		rayDirY.x = program->player.dir.y - program->player.plane.y;
		rayDirY.y = program->player.dir.y + program->player.plane.y;

		int p;
		float camZ;

		if (is_floor) {
			p = y - HEIGHT / 2 - program->player.pitch;
			camZ = 0.5 * HEIGHT + program->player.height;
		}
		else {
			p = HEIGHT / 2 - y + program->player.pitch;
			camZ = 0.5 * HEIGHT - program->player.height;
		}
		float rowDistance = camZ / p;
		t_coord floorStep;
		floorStep.x = rowDistance * (rayDirX.y - rayDirX.x) / WIDTH;
		floorStep.y = rowDistance * (rayDirY.y - rayDirY.x) / WIDTH;
		t_coord floor;
		floor.x = program->player.pos.x + rowDistance * rayDirX.x;
		floor.y = program->player.pos.y + rowDistance * rayDirY.x;
		int x = -1;

		while(++x < WIDTH)
		{
			t_coord_int cell;
			cell.x = (int)(floor.x);
			cell.y = (int)(floor.y);
			t_coord_int t;
			t.x = (int)(MAX_RES * (floor.x - cell.x)) & (MAX_RES - 1);
			t.y = (int)(MAX_RES * (floor.y - cell.y)) & (MAX_RES - 1);
			floor.x += floorStep.x;
			floor.y += floorStep.y;
			mlx_put_pixel(program->map.img, x, y, program->map.floor.rgba);
			mlx_put_pixel(program->map.img, x, HEIGHT - y - 1, program->map.ceiling.rgba);
		}
	}
}

void draw_wall(t_program *program)
{
	int x;

	x = -1;
	while (++x < WIDTH)
	{
		t_coord rayDir;
		double cameraX = 2 * x / (double)(WIDTH) - 1;
		rayDir.x = program->player.dir.x + program->player.plane.x * cameraX;
		rayDir.y = program->player.dir.y + program->player.plane.y * cameraX;

		t_coord_int map;
		map.x = (int)(program->player.pos.x);
		map.y = (int)(program->player.pos.y);

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
		int offset;
		offset = program->player.pitch + (program->player.height / perpWallDist);
		drawStart = -lineHeight / 2 + HEIGHT / 2 + offset;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + HEIGHT / 2 + offset;
		if (drawEnd >= HEIGHT)
			drawEnd = HEIGHT - 1;

		mlx_image_t	*text;
		double wallX;

		if (side == 0)
		{
			wallX = program->player.pos.y + perpWallDist * rayDir.y;
			if (program->player.pos.x < map.x)
				text = program->map.west;
			else
				text = program->map.east;
		}
		else
		{
			wallX = program->player.pos.x + perpWallDist * rayDir.x;
			if (program->player.pos.y < map.y)
				text = program->map.south;
			else
				text = program->map.north;
		}

		wallX -= floor(wallX);
		t_coord_int tex;

		tex.x = (int)(wallX * (double)(MAX_RES));
		if (side == 0 && rayDir.x > 0)
			tex.x = MAX_RES - tex.x - 1;
		else if (side == 1 && rayDir.y < 0)
			tex.x = MAX_RES - tex.x - 1;

		double texStep;
		double texPos;
		int y;

		texStep = 1.0 * MAX_RES / lineHeight;
		texPos = (drawStart - program->player.pitch - (program->player.height / perpWallDist) - HEIGHT / 2 + lineHeight / 2) * texStep;
		/*
		y = drawStart - 1;
		while (++y < drawEnd)
		{
			tex.y = (int)(texPos) & (MAX_RES - 1);
			texPos += texStep;
			uint32_t coord = (MAX_RES * tex.y + tex.x) * 4;
			uint32_t color = get_color_rgba(text->pixels[coord], text->pixels[coord + 1], text->pixels[coord + 2], text->pixels[coord + 3]);
			mlx_put_pixel(program->map.img, x, y, color);
		}
		*/

		//draw floor
		t_coord floorWall;
		if (side == 0 && rayDir.x > 0)
		{
			floorWall.x = map.x;
			floorWall.y = map.y + wallX;
		}
		else if (side == 0 && rayDir.x < 0)
		{
			floorWall.x = map.x + 1.0;
			floorWall.y = map.y + wallX;
		}
		else if (side == 1 && rayDir.x > 0)
		{
			floorWall.x = map.x + wallX;
			floorWall.y = map.y;
		}
		else
		{
			floorWall.x = map.x + wallX;
			floorWall.y = map.y + 1.0;
		}

		double distWall;
		double distPlayer;
		double currentDist;
		distWall = perpWallDist;
		distPlayer = 0.0;
		if (drawEnd < 0)
			drawEnd = HEIGHT;
		y = -1;
		while(++y <= drawStart)
		{
			currentDist = (HEIGHT - (2.0 * program->player.height)) / (HEIGHT * 2.0 * (y - program->player.pitch));
			double weight;
			weight = (currentDist - distPlayer) / (distWall - distPlayer);
			t_coord currentFloor;
			currentFloor.x = weight * floorWall.x + (1.0 - weight) * program->player.pos.x;
			currentFloor.y = weight * floorWall.y + (1.0 - weight) * program->player.pos.y;
			mlx_put_pixel(program->map.img, x, y, program->map.ceiling.rgba);
		}
		y = drawEnd - 1;
		while(++y < HEIGHT)
		{
			currentDist = (HEIGHT + (2.0 * program->player.height)) / (2.0 * (y - program->player.pitch) - HEIGHT);
			double weight;
			weight = (currentDist - distPlayer) / (distWall - distPlayer);
			t_coord currentFloor;
			currentFloor.x = weight * floorWall.x + (1.0 - weight) * program->player.pos.x;
			currentFloor.y = weight * floorWall.y + (1.0 - weight) * program->player.pos.y;
			mlx_put_pixel(program->map.img, x, y, program->map.floor.rgba);
		}

		y = drawStart - 1;
		while (++y < drawEnd)
		{
			tex.y = (int)(texPos) & (MAX_RES - 1);
			texPos += texStep;
			uint32_t coord = (MAX_RES * tex.y + tex.x) * 4;
			uint32_t color = get_color_rgba(text->pixels[coord], text->pixels[coord + 1], text->pixels[coord + 2], text->pixels[coord + 3]);
			mlx_put_pixel(program->map.img, x, y, color);
		}
	}
}

void draw(t_program *program)
{
	//draw_background_no_cast(program);
	//draw_background(program);
	draw_wall(program);
}
