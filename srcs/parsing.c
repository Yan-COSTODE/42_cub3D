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

void	parse_image(t_program *program, int *status, mlx_image_t** image, char **args)
{
	mlx_texture_t *tex;
	char *tmp;

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
	if (ft_strlen_split(args) != 2 || !image || !*image)
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

void	parse_content(t_program *program)
{
	char *tmp;
	char	*line;
	char *final;

	line = get_next_line(program->fd);
	while (line[0] == '\n')
	{
		free(line);
		line = get_next_line(program->fd);
	}
	final = line;
	line = get_next_line(program->fd);
	program->map.height++;
	while (line)
	{
		tmp = ft_strjoin(final, line);
		free(final);
		final = tmp;
		if ((int)ft_strlen(line) > program->map.width)
			program->map.width = ft_strlen(line);
		free(line);
		line = get_next_line(program->fd);
		program->map.height++;
	}
	program->map.content = ft_split(final, '\n');
	free(final);
}

int	parse_map(t_program *program, char *line)
{
	char **split;
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
		program->player.plane.x = -1;
		program->player.plane.y = 0;
	}
	else if (program->player.orientation == 'E')
	{
		program->player.dir.x = 1;
		program->player.dir.y = 0;
		program->player.plane.x = 0;
		program->player.plane.y = -1;
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

void	fill_width(t_program *program)
{
	char *spaces;
	int		line_len;
	int		i;
	int		j;

	i = -1;
	while (program->map.content[++i])
	{
		line_len = ft_strlen(program->map.content[i]);
		if (line_len < program->map.width)
		{
			spaces = ft_calloc(program->map.width + 1, sizeof(char));
			j = -1;
			while (++j < program->map.width)
			{
				if (j < line_len)
					spaces[j] = program->map.content[i][j];
				else
					spaces[j] = ' ';
			}
			free(program->map.content[i]);
			program->map.content[i] = spaces;
		}
	}
}

// TODO check if the map is valid, closed by walls and no random char

void parse(t_program *program)
{
	char	*line;
	int		n_line;

	open_file(program);
	if (program->exit_value == EXIT_FAILURE)
		return ;
	line = "";
	n_line = 0;
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
				break;
			}
			free(line);
		}
	}
	parse_player(program);
	close_file(program);
}

bool	check_sides(char *str)
{
	return (str[0] == '1' && str[ft_strlen(str) - 1] == '1');
}

int	check_first_last(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' || str[i] != '1')
			return (0);
		i++;
	}
	return (1);
}

int check_middle(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (i == 0 || i == (int)ft_strlen(str) - 1)
		{
			if (str[i] != ' ' || str[i] != '1')
				return (0);
		}
		else
			if (str[i] != ' ' || str[i] != '0' || str[i] != '1')
				return (0);
		i++;
	}
	return (1);
}

int is_space_surrounded(t_program *program, char **map, int y, int x)
{
	if (y != 0)
		if (map[y - 1][x] != '1' && map[y - 1][x] != ' ')
			return (0);
	if (x != 0)
		if (map[y][x - 1] != '1' && map[y][x - 1] != ' ')
			return (0);
	if (y != program->map.height - 1)
		if (map[y + 1][x] != '1' && map[y + 1][x] != ' ')
			return (0);
	if (x != (int)ft_strlen(map[y]) - 1)
		if (map[y][x + 1] != '1' && map[y][x + 1] != ' ')
			return (0);
	return (1);
}

int check_borders(t_program *program)
{
	int y;
	int x;

	y = 0;
	while (program->map.content[y])
	{
		x = 0;
		while (program->map.content[y][x] == ' ')
			x++;
		if (program->map.content[y][x] != '1')
			return (0);
		x = ft_strlen(program->map.content[y]) - 1;
		while (program->map.content[y][x] == ' ')
			x--;
		if (program->map.content[y][x] != '1')
			return (0);
		y++;
	}
	return (1);
}

int	parsing(t_program *program)
{
	int	y;
	int	x;
	int player;

	y = 0;
	player = 0;
	if (!check_borders(program))
	{
		print_error("Map is not fully bordered by 1's");
		return (0);
	}
	while (program->map.content[y])
	{
		x = 0;
		while (program->map.content[y][x])
		{
			if (program->map.content[y][x] == ' ')
			{
				if (!is_space_surrounded(program, program->map.content, y, x))
				{
					print_error("Space character is not surrounded by 1's");
					return (0);
				}
			}
			x++;
			if (program->map.content[y][x] == 'N' || program->map.content[y][x] == 'S'
				|| program->map.content[y][x] == 'W' || program->map.content[y][x] == 'E')
					player++;
		}
		y++;
	}
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
	return (1);
}
