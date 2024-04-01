/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 12:28:57 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/01 12:28:57 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	change_cursor(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	t_program	*program;

	(void)mods;
	program = param;
	shoot(program, button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS);
	if (button != MLX_MOUSE_BUTTON_RIGHT || action != MLX_PRESS)
		return ;
	if (program->cursor == MLX_MOUSE_HIDDEN)
		program->cursor = MLX_MOUSE_NORMAL;
	else if (program->cursor == MLX_MOUSE_NORMAL)
		program->cursor = MLX_MOUSE_HIDDEN;
}

void	init_program(t_program *program)
{
	program->exit_value = 0;
	program->map.height = 0;
	program->map.width = 0;
	program->map.content = NULL;
	program->player.orientation = 0;
	program->player.jump = false;
	program->player.pitch = 0;
	program->player.height = 0;
	program->cursor = MLX_MOUSE_HIDDEN;
	program->hud.bobbing = MAX_BOBBING;
	program->hud.direction = -1.5;
	program->hud.gindex = 0;
	program->hud.shoot = false;
	program->music = 0;
}

void	on_destroy(t_program *program)
{
	free(program->filename);
	ft_freesplit(program->map.content);
	mlx_close_window(program->mlx);
	kill(program->music, SIGTERM);
}

void	parse_to(t_program *program, mlx_image_t **image, char *path, int width, int height)
{
	mlx_texture_t	*tmp;

	tmp = mlx_load_png(path);
	if (tmp)
	{
		*image = mlx_texture_to_image(program->mlx, tmp);
		mlx_resize_image(*image, width, height);
		mlx_delete_texture(tmp);
	}
	else
		*image = NULL;
}

void	parse_const(t_program *program)
{
	parse_to(program, &program->minimap.img_player, "./textures/player.png", MINIMAP_PLAYER, MINIMAP_PLAYER);
	parse_to(program, &program->hud.crosshair, "./textures/crosshair.png", CROSSHAIR, CROSSHAIR);
	parse_to(program, &program->hud.gun[0], "./textures/gun_0.png", 399, 288);
	parse_to(program, &program->hud.gun[1], "./textures/gun_1.png", 399, 288);
	parse_to(program, &program->hud.gun[2], "./textures/gun_2.png", 399, 288);
	parse_to(program, &program->hud.gun[3], "./textures/gun_3.png", 399, 288);
	setup_shoot(program);
}

int	start(t_program *program)
{
	init_program(program);
	program->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", false);
	if (!program->mlx)
		return (print_mlxerror("cub3D"));
	parse(program);
	if (program->exit_value == EXIT_FAILURE)
		return (EXIT_FAILURE);
	program->map.img = mlx_new_image(program->mlx, WIDTH, HEIGHT);
	program->minimap.img = mlx_new_image(program->mlx, MINIMAP_SIZE * 2, MINIMAP_SIZE * 2);
	program->minimap.display = mlx_new_image(program->mlx, MINIMAP_SIZE * 2, MINIMAP_SIZE);
	mlx_image_to_window(program->mlx, program->map.img, 0, 0);
	parse_const(program);
	mlx_image_to_window(program->mlx, program->minimap.display, MINIMAP_OFFSET, MINIMAP_OFFSET);
	if (program->hud.crosshair)
		mlx_image_to_window(program->mlx, program->hud.crosshair, WIDTH / 2 - program->hud.crosshair->width / 2, HEIGHT / 2 - program->hud.crosshair->height / 2);
	if (program->minimap.img_player)
		mlx_image_to_window(program->mlx, program->minimap.img_player, MINIMAP_OFFSET + MINIMAP_SIZE - program->minimap.img_player->width / 2, MINIMAP_OFFSET + MINIMAP_SIZE / 2 - program->minimap.img_player->height / 2);
	return (EXIT_SUCCESS);
}

void	update(void *param)
{
	t_program	*program;

	program = param;
	if (mlx_is_key_down(program->mlx, MLX_KEY_ESCAPE))
		return (on_destroy(program));
	move(program);
	rotate(program);
	draw(program);
	draw_minimap(program);
	play_background(program);
	mlx_set_cursor_mode(program->mlx, program->cursor);
}

int	main(int argc, char **argv)
{
	t_program	program;

	(void)argv;
	if (argc != 2)
		return (print_error("cub3D: Wrong Number of Arguments"));
	program.filename = ft_strdup(argv[1]);
	if (start(&program) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!parsing(&program))
	{
		on_destroy((void *)&program);
		return (EXIT_FAILURE);
	}
	print_map(program.map);
	mlx_close_hook(program.mlx, (void (*)(void *))on_destroy, &program);
	mlx_mouse_hook(program.mlx, change_cursor, &program);
	mlx_loop_hook(program.mlx, update, &program);
	mlx_loop(program.mlx);
	mlx_terminate(program.mlx);
	return (EXIT_SUCCESS);
}
