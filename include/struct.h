/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 16:31:55 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/13 16:29:51 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_coord_int
{
	int	x;
	int	y;
}	t_coord_int;

typedef struct s_coord
{
	double	x;
	double	y;
}	t_coord;

typedef struct s_color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	uint32_t	rgba;
}	t_color;

typedef struct s_minimap
{
	mlx_image_t *img;
	mlx_image_t *img_player;
}	t_minimap;

typedef struct s_map
{
	mlx_texture_t	*north;
	mlx_texture_t	*south;
	mlx_texture_t	*east;
	mlx_texture_t	*west;
	t_color floor;
	t_color ceiling;
	char	*content;
	mlx_image_t	*img;
	int	width;
	int	height;
}			t_map;

typedef struct s_player
{
	t_coord plane;
	t_coord dir;
	t_coord pos;
	char orientation;
}	t_player;

typedef struct s_program
{
	mlx_t	*mlx;
	t_minimap minimap;
	t_map	map;
	t_player player;
	char	*filename;
	int		fd;
	int		exit_value;
}			t_program;

#endif
