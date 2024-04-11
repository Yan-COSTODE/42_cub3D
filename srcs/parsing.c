/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 21:01:12 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 19:54:20 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse_image(t_program *program, int *status, mlx_image_t **image,
		char **args)
{
	mlx_texture_t	*tex;
	char			*tmp;

	if (!image)
		return ;
	if (ft_strlen_split(args) == 2)
	{
		tmp = ft_substr(args[1], 0, ft_strchr(args[1], '\n') - args[1]);
		tex = mlx_load_png(tmp);
		if (tex)
		{
			*image = mlx_texture_to_image(program->mlx, tex);
			mlx_resize_image(*image, MAX_RES, MAX_RES);
			mlx_delete_texture(tex);
		}
		free(tmp);
	}
	if (ft_strlen_split(args) != 2 || !*image)
		return (parse_error(status, args[0]));
}

void	parse_color(int *status, t_color *color, char **args)
{
	char	**split;
	char	*tmp;

	if (ft_strlen_split(args) == 2)
	{
		split = ft_split(args[1], ',');
		if (ft_strlen_split(split) != 3)
			return (parse_error_color(status, args[0], split, NULL));
		if (!atoi_convert(split[0], &color->r))
			return (parse_error_color(status, args[0], split, "r"));
		if (!atoi_convert(split[1], &color->g))
			return (parse_error_color(status, args[0], split, "g"));
		tmp = ft_substr(split[2], 0, ft_strchr(split[2], '\n') - split[2]);
		if (!atoi_convert(tmp, &color->b))
		{
			free(tmp);
			return (parse_error_color(status, args[0], split, "b"));
		}
		color->rgba = get_color_rgba(color->r, color->g, color->b, 255);
		free(tmp);
		ft_freesplit(split);
	}
	if (ft_strlen_split(args) != 2)
		return (parse_error(status, args[0]));
}

int	parse_map(t_program *program, char *line)
{
	char		**split;
	static int	args = 0;

	split = ft_split(line, ' ');
	if (ft_strcmp(split[0], "NO") == 0 && !program->map.content)
		parse_image(program, &program->exit_value, &program->map.north, split);
	else if (ft_strcmp(split[0], "SO") == 0 && !program->map.content)
		parse_image(program, &program->exit_value, &program->map.south, split);
	else if (ft_strcmp(split[0], "WE") == 0 && !program->map.content)
		parse_image(program, &program->exit_value, &program->map.west, split);
	else if (ft_strcmp(split[0], "EA") == 0 && !program->map.content)
		parse_image(program, &program->exit_value, &program->map.east, split);
	else if (ft_strcmp(split[0], "F") == 0 && !program->map.content)
		parse_color(&program->exit_value, &program->map.floor, split);
	else if (ft_strcmp(split[0], "C") == 0 && !program->map.content)
		parse_color(&program->exit_value, &program->map.ceiling, split);
	else
	{
		ft_freesplit(split);
		return (args);
	}
	args++;
	ft_freesplit(split);
	return (args);
}

void	parse(t_program *program)
{
	char	*line;

	open_file(program);
	if (program->exit_value == EXIT_FAILURE)
		return ;
	line = "";
	while (line)
	{
		line = get_next_line(program->fd);
		if (line)
		{
			if (parse_map(program, line) == 6)
			{
				free(line);
				parse_content(program);
				fill_width(program);
				break ;
			}
			free(line);
		}
	}
	parse_player(program);
	close_file(program);
}

int	parsing(t_program *program)
{
	t_coord_int	index;
	int			player;

	index.y = 0;
	player = 0;
	if (!check_borders(program))
	{
		print_error("Map is not fully bordered by 1's");
		return (0);
	}
	if (parsing_utils(program, &index, &player) == 0)
		return (0);
	if (player > 1)
	{
		print_error("There can only be one spawn point");
		return (0);
	}
	if (player == 0)
	{
		print_error("No spawn point has been set");
		return (0);
	}
	parse_door(program);
	return (1);
}
