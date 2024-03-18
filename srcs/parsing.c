#include "cub3d.h"

void close_file(t_program *program)
{
	if (close(program->fd) == -1)
		program->exit_value = print_perror("cub3D: close");
}

void open_file(t_program *program)
{
	if (ft_strlen(program->filename) < 5 || ft_strcmp(&program->filename[ft_strlen(program->filename) - 4], ".cub") != 0)
	{
		program->exit_value = print_error("cub3D: file error");
		return ;
	}
	program->fd = open(program->filename, O_RDONLY);
	if (program->fd == -1)
		program->exit_value = print_perror("cub3D: open");
}

void parse_error(int *status, char *name)
{
	*status = EXIT_FAILURE;
	print_fd(2, "\x1b[1;31mError\ncub3D: ");
	print_fd(2, name);
	print_fd(2, ": Wrong Value\n\x1b[0m");
}

void parse_error_color(int *status, char *name, char **split, char *color)
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

void	parse_texture(int *status, mlx_texture_t* texture, char **args)
{
	char *tmp;

	if (ft_strlen_split(args) == 2)
	{
		tmp = ft_substr(args[1], 0, ft_strchr(args[1], '\n') - args[1]);
		texture = mlx_load_png(tmp);
		free(tmp);
	}
	if (ft_strlen_split(args) != 2 || !texture)
		return (parse_error(status, args[0]));
}

uint32_t get_color_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	parse_color(int *status, t_color* color, char **args)
{
	char **split;
	char *tmp;

	if (ft_strlen_split(args) == 2)
	{
		split = ft_split(args[1], ',');
		if (ft_strlen_split(split) != 3)
			return (parse_error_color(status, args[0], split, NULL));
		if (!atoi_convert(split[0], &color->r))
			return (parse_error_color(status, args[0], split, "r"));
		if (!atoi_convert(split[1], &color->g))
			return (parse_error_color(status, args[0], split, "g"));
		{
			tmp = ft_substr(split[2], 0, ft_strchr(split[2], '\n') - split[2]);
			if (!atoi_convert(tmp, &color->b))
			{
				free(tmp);
				return (parse_error_color(status, args[0], split, "b"));
			}
			color->rgba = get_color_rgba(color->r, color->g, color->b, 255);
			free(tmp);
		}
		ft_freesplit(split);
	}
	if (ft_strlen_split(args) != 2)
		return (parse_error(status, args[0]));
}

bool is_empty_line(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\n' && line[i] != '\t' && line[i] != '\v' && line[i] != '\f' && line[i] != '\r')
			return (false);
		++i;
	}
	return (true);
}

void	parse_content(t_map *map, char *line)
{
	char *tmp;
	char *final;

	if (!map->content && is_empty_line(line))
		return ;
	tmp = ft_substr(line, 0, ft_strchr(line, '\n') - line);
	if ((int)ft_strlen(tmp) > map->width)
		map->width = ft_strlen(tmp);
	final = ft_strjoin(map->content, tmp);
	if (map->content)
		free(map->content);
	free(tmp);
	map->content = final;
	++map->height;
}

void	parse_map(t_program *program, char *line)
{
	char **split;

	split = ft_split(line, ' ');
	if (ft_strcmp(split[0], "NO") == 0 && !program->map.content)
		parse_texture(&program->exit_value, program->map.north, split);
	else if (ft_strcmp(split[0], "SO") == 0 && !program->map.content)
		parse_texture(&program->exit_value, program->map.south, split);
	else if (ft_strcmp(split[0], "WE") == 0 && !program->map.content)
		parse_texture(&program->exit_value, program->map.west, split);
	else if (ft_strcmp(split[0], "EA") == 0 && !program->map.content)
		parse_texture(&program->exit_value, program->map.east, split);
	else if (ft_strcmp(split[0], "F") == 0 && !program->map.content)
		parse_color(&program->exit_value, &program->map.floor, split);
	else if (ft_strcmp(split[0], "C") == 0 && !program->map.content)
		parse_color(&program->exit_value, &program->map.ceiling, split);
	else
		parse_content(&program->map, line);
	ft_freesplit(split);
}

void set_player_orientation(t_program *program)
{
	if (program->player.orientation == 'N')
	{
		program->player.dir.x = 0;
		program->player.dir.y = 1;
		program->player.plane.x = 1;
		program->player.plane.y = 0;
	}
	else if (program->player.orientation == 'S')
	{
		program->player.dir.x = 0;
		program->player.dir.y = -1;
		program->player.plane.x = 1;
		program->player.plane.y = 0;
	}
	else if (program->player.orientation == 'E')
	{
		program->player.dir.x = 1;
		program->player.dir.y = 0;
		program->player.plane.x = 0;
		program->player.plane.y = 1;
	}
	else if (program->player.orientation == 'W')
	{
		program->player.dir.x = -1;
		program->player.dir.y = 0;
		program->player.plane.x = 0;
		program->player.plane.y = 1;
	}
}

void parse_player(t_program *program)
{
	int x;
	int y;

	y = -1;
	while (++y < program->map.height)
	{
		x = -1;
		while (++x < program->map.width)
		{
			if (get_at(program->map, x, y) == 'N')
				program->player.orientation = 'N';
			if (get_at(program->map, x, y) == 'S')
				program->player.orientation = 'S';
			if (get_at(program->map, x, y) == 'E')
				program->player.orientation = 'E';
			if (get_at(program->map, x, y) == 'W')
				program->player.orientation = 'W';
			if (program->player.orientation != 0)
			{
				program->player.pos.x = x + 0.5;
				program->player.pos.y = y + 0.5;
				set_player_orientation(program);
				return ;
			}
		}
	}
}

// TODO check if the map is valid, closed by walls and no random char

void parse(t_program *program)
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
			parse_map(program, line);
			free(line);
			line = "";
		}
	}
	parse_player(program);
	close_file(program);
}