#include "cub3d.h"

void init_program(t_program *program)
{
	program->exit_value = 0;
	program->map.height = 0;
	program->map.width = 0;
	program->map.content = NULL;
	program->player.orientation = 0;
}

void on_destroy(t_program *program)
{
	mlx_close_window(program->mlx);
	mlx_delete_texture(program->map.north);
	mlx_delete_texture(program->map.south);
	mlx_delete_texture(program->map.east);
	mlx_delete_texture(program->map.west);
}

int start(t_program *program)
{
	init_program(program);
	parse(program);
	if (program->exit_value == EXIT_FAILURE)
		return (EXIT_FAILURE);
	program->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", false);
	if (!program->mlx)
		return (print_mlxerror("cub3D"));
	program->map.img = mlx_new_image(program->mlx, WIDTH, HEIGHT);
	mlx_image_to_window(program->mlx, program->map.img, 0, 0);
	return (EXIT_SUCCESS);
}

void update(void *param)
{
	static mlx_image_t *img = NULL;
	t_program	*program;
	char	fps[100];

	program = param;
	if (mlx_is_key_down(program->mlx, MLX_KEY_ESCAPE))
		on_destroy(program);
	//raycast(program);
	if (img)
		mlx_delete_image(program->mlx, img);
	sprintf(fps, "%.0f FPS", 1 / program->mlx->delta_time);
	img = mlx_put_string(program->mlx, fps, 0, 0);
}

int main(int argc, char **argv)
{
	t_program program;

	(void)argv;
	if (argc != 2)
		return (print_error("cub3D: Wrong Number of Arguments"));
	program.filename = ft_strdup(argv[1]);
	if (start(&program) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	print_map(program.map);
	raycast(&program);
	mlx_key_hook(program.mlx, (void (*)(mlx_key_data_t, void *))
		move, &program);
	mlx_loop_hook(program.mlx, update, &program);
	mlx_loop(program.mlx);
	mlx_terminate(program.mlx);
	return (EXIT_SUCCESS);
}