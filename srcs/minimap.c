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
}