/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:32:06 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:33:02 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse_content(t_program *program)
{
	t_parse	parse;

	parse.line = get_next_line(program->fd);
	while (parse.line[0] == '\n')
	{
		free(parse.line);
		parse.line = get_next_line(program->fd);
	}
	parse.final = parse.line;
	parse.line = get_next_line(program->fd);
	while (parse.line)
	{
		if (parse.line[0] == '\n')
		{
			free(parse.line);
			parse.line = ft_strdup(" \n");
		}
		parse.tmp = ft_strjoin(parse.final, parse.line);
		free(parse.final);
		parse.final = parse.tmp;
		if ((int)ft_strlen(parse.line) - 1 > program->map.width)
			program->map.width = ft_strlen(parse.line) - 1;
		free(parse.line);
		parse.line = get_next_line(program->fd);
	}
	program->map.content = ft_split(parse.final, '\n');
	program->map.height = ft_strlen_split(program->map.content);
	free(parse.final);
}

void	fill_width(t_program *program)
{
	char	*spaces;
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

int	is_space_surrounded(t_program *program, char **map, int y, int x)
{
	if (y != 0)
		if (map[y - 1][x] != '1' && map[y - 1][x] != ' ')
			return (0);
	if (x != 0)
		if (map[y][x - 1] != '1' && map[y][x - 1] != ' ')
			return (0);
	if (y < program->map.height)
		if (map[y + 1][x] != '1' && map[y + 1][x] != ' ')
			return (0);
	if (x < (int)ft_strlen(map[y]))
		if (map[y][x + 1] != '1' && map[y][x + 1] != ' ')
			return (0);
	return (1);
}

int	check_borders(t_program *program)
{
	int	y;
	int	x;

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
