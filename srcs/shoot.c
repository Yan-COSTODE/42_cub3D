#include "cub3d.h"

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
		program->hud.timer += program->mlx->delta_time;
		if (program->hud.timer >= ANIM_DELAY)
		{
			++program->hud.gindex;
			if (program->hud.gindex > 3)
			{
				program->hud.gindex = 0;
				program->hud.shoot = false;
			}
			program->hud.timer = 0;
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

	program->hud.bobbing += program->hud.direction;
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
		program->hud.timer = 0;
	}
}