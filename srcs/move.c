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

void move_down(t_program *program, double move_speed)
{
	int x;
	int y;

	x = program->player.pos.x - program->player.dir.x * move_speed;
	y = program->player.pos.y - program->player.dir.y * move_speed;

	if (get_at(program->map, x, program->player.pos.y) != '1')
		program->player.pos.x -= program->player.dir.x * move_speed;
	if (get_at(program->map, program->player.pos.x, y) != '1')
		program->player.pos.y -= program->player.dir.y * move_speed;
}

void move_left(t_program *program, double move_speed)
{
	int x;
	int y;

	x = program->player.pos.x + program->player.dir.y * move_speed;
	y = program->player.pos.y - program->player.dir.x * move_speed;

	if (get_at(program->map, x, program->player.pos.y) != '1')
		program->player.pos.x += program->player.dir.y * move_speed;
	if (get_at(program->map, program->player.pos.x, y) != '1')
		program->player.pos.y -= program->player.dir.x * move_speed;
}

void move_right(t_program *program, double move_speed)
{
	int x;
	int y;

	x = program->player.pos.x - program->player.dir.y * move_speed;
	y = program->player.pos.y + program->player.dir.x * move_speed;

	if (get_at(program->map, x, program->player.pos.y) != '1')
		program->player.pos.x -= program->player.dir.y * move_speed;
	if (get_at(program->map, program->player.pos.x, y) != '1')
		program->player.pos.y += program->player.dir.x * move_speed;
}

void rotate_left(t_program *program, double rotate_speed)
{
	double oldDirX;
	double oldPlaneX;

	oldDirX = program->player.dir.x;
	program->player.dir.x = program->player.dir.x * cos(-rotate_speed) - program->player.dir.y * sin(-rotate_speed);
	program->player.dir.y = oldDirX * sin(-rotate_speed) + program->player.dir.y * cos(-rotate_speed);
	oldPlaneX = program->player.plane.x;
	program->player.plane.x = program->player.plane.x * cos(-rotate_speed) - program->player.plane.y * sin(-rotate_speed);
	program->player.plane.y = oldPlaneX * sin(-rotate_speed) + program->player.plane.y * cos(-rotate_speed);
}

void rotate_right(t_program *program, double rotate_speed)
{
	double oldDirX;
	double oldPlaneX;

	oldDirX = program->player.dir.x;
	program->player.dir.x = program->player.dir.x * cos(rotate_speed) - program->player.dir.y * sin(rotate_speed);
	program->player.dir.y = oldDirX * sin(rotate_speed) + program->player.dir.y * cos(rotate_speed);
	oldPlaneX = program->player.plane.x;
	program->player.plane.x = program->player.plane.x * cos(rotate_speed) - program->player.plane.y * sin(rotate_speed);
	program->player.plane.y = oldPlaneX * sin(rotate_speed) + program->player.plane.y * cos(rotate_speed);
}

void rotate(t_program *program)
{
	t_coord_int mouse;
	double rotate_speed;

	rotate_speed = program->mlx->delta_time * ROTATE_SPEED;
	if (mlx_is_key_down(program->mlx, MLX_KEY_RIGHT))
		rotate_right(program, rotate_speed);
	if (mlx_is_key_down(program->mlx, MLX_KEY_LEFT))
		rotate_left(program, rotate_speed);
	mlx_get_mouse_pos(program->mlx, &mouse.x, &mouse.y);
	if (mouse.x > WIDTH / 2)
		rotate_right(program, rotate_speed);
	else if (mouse.x < WIDTH / 2)
		rotate_left(program, rotate_speed);
	mlx_set_mouse_pos(program->mlx, WIDTH / 2, HEIGHT / 2);
}

void move(t_program *program)
{
	double move_speed;

	move_speed = program->mlx->delta_time * MOVE_SPEED;
	if (mlx_is_key_down(program->mlx, MLX_KEY_W))
		move_up(program, move_speed);
	if (mlx_is_key_down(program->mlx, MLX_KEY_S))
		move_down(program, move_speed);
	if (mlx_is_key_down(program->mlx, MLX_KEY_A))
		move_left(program, move_speed);
	if (mlx_is_key_down(program->mlx, MLX_KEY_D))
		move_right(program, move_speed);
}