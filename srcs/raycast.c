/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 13:32:29 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/29 13:32:29 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

uint32_t set_fog(uint32_t original, double scale)
{
	t_color fog_color;
	t_color color;
	double elem;

	if (scale > MAX_FOG)
		scale = MAX_FOG;
	else if (scale < 0)
		scale = 0;

	fog_color.r = 0;
	fog_color.g = 0;
	fog_color.b = 0;
	elem = (original & RED_MASK) >> 24;
	color.r = (double)(fog_color.r) * scale + elem * (1.0 - scale);
	elem = (original & GREEN_MASK) >> 16;
	color.g = (double)(fog_color.g) * scale + elem * (1.0 - scale);
	elem = (original & BLUE_MASK) >> 8;
	color.b = (double)(fog_color.b) * scale + elem * (1.0 - scale);
	return (get_color_rgba(color.r, color.g, color.b, 255));
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
			if (get_at(program->map, map.x, map.y) == '1')
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

		double currentDist;
		if (drawEnd < 0)
			drawEnd = HEIGHT;
		y = -1;
		while(++y <= drawStart)
		{
			currentDist = (HEIGHT - (2.0 * program->player.height)) / (HEIGHT - 2.0 * (y - program->player.pitch));
			uint32_t color = set_fog(program->map.ceiling.rgba, currentDist / FOG_LENGTH);
			mlx_put_pixel(program->map.img, x, y, color);
		}
		y = drawEnd - 1;
		while(++y < HEIGHT)
		{
			currentDist = (HEIGHT + (2.0 * program->player.height)) / (2.0 * (y - program->player.pitch) - HEIGHT);
			uint32_t color = set_fog(program->map.floor.rgba, currentDist / FOG_LENGTH);
			mlx_put_pixel(program->map.img, x, y, color);
		}

		y = drawStart - 1;
		while (++y < drawEnd)
		{
			tex.y = (int)(texPos) & (MAX_RES - 1);
			texPos += texStep;
			uint32_t coord = (MAX_RES * tex.y + (MAX_RES - tex.x)) * 4;
			uint32_t color = get_color_rgba(text->pixels[coord], text->pixels[coord + 1], text->pixels[coord + 2], text->pixels[coord + 3]);
			color = set_fog(color, perpWallDist / (FOG_LENGTH * 1.15));
			mlx_put_pixel(program->map.img, x, y, color);
		}
	}
}

void draw_door(t_program *program)
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
			if (get_at(program->map, map.x, map.y) != '0')
				hit = 1;
		}
		if (get_at(program->map, map.x, map.y) == '1' || get_at(program->map, map.x, map.y) == ' ')
			continue;
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

		text = NULL;
		if (side == 0)
		{
			wallX = program->player.pos.y + perpWallDist * rayDir.y;
			if (get_at(program->map, map.x, map.y) != '1' && get_at(program->map, map.x, map.y) != ' ')
				text = program->door.img;
		}
		else
		{
			wallX = program->player.pos.x + perpWallDist * rayDir.x;
			if (get_at(program->map, map.x, map.y) != '1' && get_at(program->map, map.x, map.y) != ' ')
				text = program->door.img;
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

		if (drawEnd < 0)
			drawEnd = HEIGHT;

		y = drawEnd - (drawEnd - drawStart) * get_door(program, map) - 1;
		if (!text)
			continue;
		while (++y < drawEnd)
		{
			tex.y = (int)(texPos) & (MAX_RES - 1);
			texPos += texStep;
			uint32_t coord = (MAX_RES * tex.y + (MAX_RES - tex.x)) * 4;
			uint32_t color = get_color_rgba(text->pixels[coord], text->pixels[coord + 1], text->pixels[coord + 2], text->pixels[coord + 3]);
			color = set_fog(color, perpWallDist / (FOG_LENGTH * 1.15));
			mlx_put_pixel(program->map.img, x, y, color);
		}
	}
}

void draw(t_program *program)
{
	draw_wall(program);
	draw_door(program);
}
