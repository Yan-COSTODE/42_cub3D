/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shoot.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:24 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/29 16:13:24 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void update_door(t_program *program)
{
	int i;

	i = -1;
	while (++i < program->door.len)
	{
		if (program->door.elem[i].status >= CLOSING && program->door.elem[i].status < CLOSING + 1)
		{
			program->door.elem[i].status += DOOR_CLOSE * program->mlx->delta_time;
			if (program->door.elem[i].status >= CLOSING + 1)
			{
				program->door.elem[i].status = CLOSED;
				set_at(&program->map, program->door.elem[i].pos.x, program->door.elem[i].pos.y, (int)(CLOSED) + '0');
				play_door_boom();
			}
		}
		else if (program->door.elem[i].status >= OPENING && program->door.elem[i].status < OPENING + 1)
		{
			program->door.elem[i].status += DOOR_OPEN * program->mlx->delta_time;
			if (program->door.elem[i].status >= OPENING + 1)
			{
				program->door.elem[i].status = OPEN;
				set_at(&program->map, program->door.elem[i].pos.x, program->door.elem[i].pos.y, (int)(OPEN) + '0');
				play_door_boom();
			}
		}
	}
}

double get_door(t_program *program, t_coord_int coord)
{
	int i;

	i = -1;
	while (++i < program->door.len)
	{
		if (program->door.elem[i].pos.x == coord.x && program->door.elem[i].pos.y == coord.y)
		{
			if (program->door.elem[i].status >= CLOSING && program->door.elem[i].status < CLOSING + 1)
				return (program->door.elem[i].status - CLOSING);
			else if (program->door.elem[i].status >= OPENING && program->door.elem[i].status < OPENING + 1)
				return ((OPENING + 1)  - program->door.elem[i].status);
			else if (program->door.elem[i].status == OPEN)
				return (0.0);
			else if (program->door.elem[i].status == CLOSED)
				return (1.0);
		}
	}
	return (0.0);
}

void set_door(t_program *program, t_coord_int coord)
{
	int i;

	i = -1;
	while (++i < program->door.len)
	{
		if (program->door.elem[i].pos.x == coord.x && program->door.elem[i].pos.y == coord.y)
		{
			if (program->door.elem[i].status == CLOSED)
			{
				program->door.elem[i].status = OPENING;
				set_at(&program->map, program->door.elem[i].pos.x, program->door.elem[i].pos.y, (int)(OPENING) + '0');
				play_door();
			}
			else if (program->door.elem[i].status == OPEN)
			{
				program->door.elem[i].status = CLOSING;
				set_at(&program->map, program->door.elem[i].pos.x, program->door.elem[i].pos.y, (int)(CLOSING) + '0');
				play_door();
			}
		}
	}
}

void change_door(t_program *program)
{
	int hit;
	t_coord sideDist;
	t_coord deltaDist;
	t_coord_int map;
	t_coord_int step;
	t_coord rayDir;

	if (program->player.pitch > 150 || program->player.pitch < -150)
		return ;
	double cameraX = 2 * (WIDTH / 2) / (double)(WIDTH) - 1;
	rayDir.x = program->player.dir.x + program->player.plane.x * cameraX;
	rayDir.y = program->player.dir.y + program->player.plane.y * cameraX;

	map.x = (int)(program->player.pos.x);
	map.y = (int)(program->player.pos.y);
	if (rayDir.x == 0)
		deltaDist.x = BIG;
	else
		deltaDist.x = fabs(1 / rayDir.x);
	if (rayDir.y == 0)
		deltaDist.y = BIG;
	else
		deltaDist.y = fabs(1 / rayDir.y);

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

	hit = 0;
	while (hit == 0)
	{
		if (sideDist.x < sideDist.y)
		{
			sideDist.x += deltaDist.x;
			map.x += step.x;
		}
		else
		{
			sideDist.y += deltaDist.y;
			map.y += step.y;
		}
		if (get_at(program->map, map.x, map.y) != '0')
			hit = 1;
	}
	if (get_at(program->map, map.x, map.y) == '1')
		return;
	set_door(program, map);
}

void setup_shoot(t_program *program)
{
	int i;

	i = -1;
	while (++i <= 3)
	{
		if (!program->hud.gun[i])
			continue;
		mlx_image_to_window(program->mlx, program->hud.gun[i], WIDTH * 0.875 - program->hud.gun[i]->width, HEIGHT - program->hud.gun[i]->height + MAX_BOBBING);
		program->hud.gun[i]->enabled = false;
	}
}

void shoot_timer(t_program *program)
{
	if (program->hud.shoot)
	{
		program->timer.gun += program->mlx->delta_time;
		if (program->timer.gun >= ANIM_DELAY)
		{
			++program->hud.gindex;
			if (program->hud.gindex == 1)
			{
				play_gun();
				change_door(program);
			}
			if (program->hud.gindex > 3)
			{
				program->hud.gindex = 0;
				program->hud.shoot = false;
			}
			program->timer.gun = 0;
		}
	}
}

void show_shoot_frame(t_program *program)
{
	int i;

	i = -1;
	while (++i <= 3)
	{
		if (!program->hud.gun[i] || i == program->hud.gindex)
			continue;
		program->hud.gun[i]->enabled = false;
	}

	program->hud.bobbing += program->hud.direction * program->mlx->delta_time;
	if (program->hud.bobbing >= MAX_BOBBING)
	{
		program->hud.bobbing = MAX_BOBBING;
		program->hud.direction *= -1;
	}
	else if (program->hud.bobbing <= 0)
	{
		program->hud.bobbing = 0;
		program->hud.direction *= -1;
	}

	program->hud.gun[program->hud.gindex]->enabled = true;
	program->hud.gun[program->hud.gindex]->instances[0].y = HEIGHT - program->hud.gun[program->hud.gindex]->height + program->hud.bobbing;
	shoot_timer(program);
}

void shoot(t_program *program, bool try)
{
	if (try && !program->hud.shoot)
	{
		program->hud.shoot = true;
		program->timer.gun = 0;
	}
}