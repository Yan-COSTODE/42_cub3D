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
	free(program->filename);
	free(program->map.content);
	if (program->map.north)
		mlx_delete_texture(program->map.north);
	if (program->map.south)
		mlx_delete_texture(program->map.south);
	if (program->map.east)
		mlx_delete_texture(program->map.east);
	if (program->map.west)
		mlx_delete_texture(program->map.west);
	mlx_close_window(program->mlx);
}

void parse_minimap(t_program *program)
{
	mlx_texture_t *tmp;

	tmp = mlx_load_png("./textures/player.png");
	if (tmp)
	{
		program->minimap.img_player = mlx_texture_to_image(program->mlx, tmp);
		mlx_resize_image(program->minimap.img_player, MINIMAP_PLAYER, MINIMAP_PLAYER);
		mlx_delete_texture(tmp);
	}
	else
		program->minimap.img_player = NULL;
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
	program->minimap.img = mlx_new_image(program->mlx, MINIMAP_SIZE * 2, MINIMAP_SIZE);
	parse_minimap(program);
	mlx_image_to_window(program->mlx, program->map.img, 0, 0);
	mlx_image_to_window(program->mlx, program->minimap.img, MINIMAP_OFFSET, MINIMAP_OFFSET);
	if (program->minimap.img_player)
		mlx_image_to_window(program->mlx, program->minimap.img_player, MINIMAP_OFFSET + MINIMAP_SIZE - program->minimap.img_player->width / 2, MINIMAP_OFFSET + MINIMAP_SIZE / 2 - program->minimap.img_player->height / 2);
	return (EXIT_SUCCESS);
}

void update(void *param)
{
	t_program	*program;

	program = param;
	if (mlx_is_key_down(program->mlx, MLX_KEY_ESCAPE))
		return (on_destroy(program));
	move(program);
	rotate(program);
	draw(program);
	draw_minimap(program);
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
	mlx_close_hook(program.mlx, (void (*)(void *))on_destroy, &program);
	mlx_loop_hook(program.mlx, update, &program);
	mlx_loop(program.mlx);
	mlx_terminate(program.mlx);
	return (EXIT_SUCCESS);
}