/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:03:07 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 17:02:40 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_program(t_program *program)
{
	program->exit_value = 0;
	program->player.orientation = 0;
	program->player.jump = false;
	program->player.pitch = 0;
	program->player.height = 0;
	program->cursor = MLX_MOUSE_HIDDEN;
	program->hud.bobbing = MAX_BOBBING;
	program->hud.direction = -35;
	program->hud.gindex = 0;
	program->hud.shoot = false;
	program->timer.music = 0;
	program->timer.footstep = 0;
	program->timer.gun = 0;
	program->timer.breath = 0;
	program->player.moving = false;
	program->player.crouch = false;
	program->player.sprint = false;
	program->door.len = 0;
	program->door.elem = NULL;
	program->map.content = NULL;
	program->map.height = 0;
	program->map.width = 0;
	program->map.floor.rgba = -1;
	program->map.ceiling.rgba = -1;
}

mlx_image_t	*parse_to(t_program *program, char *path, int width, int height)
{
	mlx_texture_t	*tmp;
	mlx_image_t		*img;

	tmp = mlx_load_png(path);
	if (tmp)
	{
		img = mlx_texture_to_image(program->mlx, tmp);
		mlx_resize_image(img, width, height);
		mlx_delete_texture(tmp);
	}
	else
		img = NULL;
	return (img);
}

void	img_setup(t_program *program)
{
	double	mult;

	mult = (HEIGHT / 360.0 + 1.0 + WIDTH / 640.0 + 1.0) / 2.0;
	mlx_image_to_window(program->mlx, program->map.img, 0, 0);
	mlx_image_to_window(program->mlx, program->minimap.display, MINIMAP_OFFSET,
		MINIMAP_OFFSET);
	mlx_image_to_window(program->mlx, program->minimap.img, WIDTH / 2
		- program->minimap.img->width / 2, HEIGHT / 2
		- program->minimap.img->height / 2);
	program->minimap.img->enabled = false;
	program->minimap.img_player = parse_to(program, "./textures/player.png",
			MINIMAP_PLAYER, MINIMAP_PLAYER);
	program->door.img = parse_to(program, "./textures/door.png", MAX_RES,
			MAX_RES);
	program->hud.crosshair = parse_to(program, "./textures/crosshair.png",
			CROSSHAIR, CROSSHAIR);
	program->hud.gun[0] = parse_to(program, "./textures/gun_0.png", 133 * mult,
			96 * mult);
	program->hud.gun[1] = parse_to(program, "./textures/gun_1.png", 133 * mult,
			96 * mult);
	program->hud.gun[2] = parse_to(program, "./textures/gun_2.png", 133 * mult,
			96 * mult);
	program->hud.gun[3] = parse_to(program, "./textures/gun_3.png", 133 * mult,
			96 * mult);
	setup_shoot(program);
}

void	change_cursor(mouse_key_t button, action_t action, modifier_key_t mods,
		void *param)
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

void	switch_map(mlx_key_data_t keydata, void *param)
{
	t_program	*program;

	program = param;
	if (keydata.key == MLX_KEY_COMMA && keydata.action == MLX_PRESS)
	{
		program->minimap.img->enabled = !program->minimap.img->enabled;
		program->minimap.display->enabled = !program->minimap.display->enabled;
		program->hud.crosshair->enabled = program->minimap.display->enabled;
		if (program->minimap.display->enabled)
		{
			program->minimap.img_player->instances[0].x = MINIMAP_OFFSET
				+ MINIMAP_W / 2 - program->minimap.img_player->width / 2;
			program->minimap.img_player->instances[0].y = MINIMAP_OFFSET
				+ MINIMAP_H / 2 - program->minimap.img_player->height / 2;
		}
		else
		{
			program->minimap.img_player->instances[0].x = WIDTH / 2
				- program->minimap.img_player->width / 2;
			program->minimap.img_player->instances[0].y = HEIGHT / 2
				- program->minimap.img_player->height / 2;
		}
	}
}
