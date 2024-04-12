/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:04:52 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 17:46:40 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	color_array(char **args)
{
	char	*tmp;
	char	*pretmp;
	int		i;

	if (ft_strlen_split(args) == 2)
		return ;
	i = 0;
	tmp = NULL;
	while (args[++i])
	{
		pretmp = tmp;
		tmp = ft_strjoin(pretmp, args[i]);
		free(pretmp);
		free(args[i]);
	}
	args[1] = tmp;
	args[2] = NULL;
}

void	parse_color(int *status, t_color *color, char **args)
{
	char	**split;
	char	*tmp;

	color_array(args);
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

void	flood_fill(t_program *program, char **map, int x, int y)
{
	if (y < 0 || y >= program->map.height || x < 0 || x >= program->map.width
		|| map[y][x] != '0')
		return ;
	map[y][x] = '.';
	flood_fill(program, map, x - 1, y);
	flood_fill(program, map, x + 1, y);
	flood_fill(program, map, x, y - 1);
	flood_fill(program, map, x, y + 1);
}
