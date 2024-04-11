/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:32:06 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:32:57 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	close_file(t_program *program)
{
	if (close(program->fd) == -1)
		program->exit_value = print_perror("cub3D: close");
}

void	open_file(t_program *program)
{
	if (ft_strlen(program->filename) < 5
		|| ft_strcmp(&program->filename[ft_strlen(program->filename) - 4],
			".cub") != 0)
	{
		program->exit_value = print_error("cub3D: file error");
		return ;
	}
	program->fd = open(program->filename, O_RDONLY);
	if (program->fd == -1)
		program->exit_value = print_perror("cub3D: open");
}

void	parse_error(int *status, char *name)
{
	*status = EXIT_FAILURE;
	print_fd(2, "\x1b[1;31mError\ncub3D: ");
	print_fd(2, name);
	print_fd(2, ": Wrong Value\n\x1b[0m");
}

void	parse_error_color(int *status, char *name, char **split, char *color)
{
	ft_freesplit(split);
	*status = EXIT_FAILURE;
	print_fd(2, "\x1b[1;31mError\ncub3D: ");
	print_fd(2, name);
	if (color)
	{
		print_fd(2, ": ");
		print_fd(2, color);
	}
	print_fd(2, ": Wrong Value\n\x1b[0m");
}

uint32_t	get_color_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}
