/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:03:15 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 17:02:52 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	program->minimap.img = mlx_new_image(program->mlx, MAP_SIZE, MAP_SIZE);
	program->minimap.display = mlx_new_image(program->mlx, MINIMAP_W,
			MINIMAP_H);
	img_setup(program);
	if (program->hud.crosshair)
		mlx_image_to_window(program->mlx, program->hud.crosshair, WIDTH / 2
			- program->hud.crosshair->width / 2, HEIGHT / 2
			- program->hud.crosshair->height / 2);
	if (program->minimap.img_player)
		mlx_image_to_window(program->mlx, program->minimap.img_player,
			MINIMAP_OFFSET + MINIMAP_W / 2 - program->minimap.img_player->width
			/ 2, MINIMAP_OFFSET + MINIMAP_H / 2
			- program->minimap.img_player->height / 2);
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
	update_sound(program);
	update_door(program);
	mlx_set_cursor_mode(program->mlx, program->cursor);
}

void	on_destroy(t_program *program)
{
	free(program->filename);
	ft_freesplit(program->map.content);
	mlx_close_window(program->mlx);
	if (program->door.elem)
		free(program->door.elem);
	if (program->timer.music != 0)
		system("killall paplay");
}
