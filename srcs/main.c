/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 12:28:57 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 18:12:04 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_program	program;

	(void)argv;
	if (argc != 2)
		return (print_error("cub3D: Wrong Number of Arguments"));
	program.filename = ft_strdup(argv[1]);
	if (start(&program) == EXIT_FAILURE)
	{
		on_destroy(&program);
		return (EXIT_FAILURE);
	}
	if (parsing(&program))
	{
		on_destroy(&program);
		return (EXIT_FAILURE);
	}
	mlx_set_mouse_pos(program.mlx, WIDTH / 2, HEIGHT / 2);
	mlx_close_hook(program.mlx, (void (*)(void *))on_destroy, &program);
	mlx_mouse_hook(program.mlx, change_cursor, &program);
	mlx_key_hook(program.mlx, switch_map, &program);
	mlx_loop_hook(program.mlx, update, &program);
	mlx_loop(program.mlx);
	mlx_terminate(program.mlx);
	return (EXIT_SUCCESS);
}
