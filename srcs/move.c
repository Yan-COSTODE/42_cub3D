#include "cub3d.h"

void move_up(t_program *program, double move_speed)
{
	int x;
	int y;

	x = program->player.pos.x + program->player.dir.x * move_speed;
	y = program->player.pos.y + program->player.dir.y * move_speed;

	if (get_at(program->map, x, program->player.pos.y) != '1')
		program->player.pos.x += program->player.dir.x * move_speed;
	if (get_at(program->map, program->player.pos.x, y) != '1')
		program->player.pos.y += program->player.dir.y * move_speed;
}

void move(mlx_key_data_t keydata, t_program *program)
{
	double move_speed;
	double rotate_speed;

	move_speed = program->mlx->delta_time * MOVE_SPEED;
	rotate_speed = program->mlx->delta_time * ROTATE_SPEED;
	if ((keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT) && keydata.action == MLX_PRESS)
		move_up(program, move_speed);
}