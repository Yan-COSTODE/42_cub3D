/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 16:31:55 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/25 12:02:03 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef enum s_door_status
{
	CLOSED = 2,
	OPENING = 3,
	OPEN = 4,
	CLOSING = 5
} t_door_status;

typedef struct s_coord_int
{
	int	x;
	int	y;
}	t_coord_int;

typedef struct s_door_elem
{
	t_coord_int pos;
	double status;
} t_door_elem;

typedef struct s_door
{
	t_door_elem *elem;
	int len;
	mlx_image_t *img;
}	t_door;

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
	unsigned char a;
	uint32_t	rgba;
}	t_color;

typedef struct s_hud
{
	mlx_image_t *crosshair;
	mlx_image_t *gun[4];
	int gindex;
	double bobbing;
	double direction;
	bool shoot;
}	t_hud;

typedef struct s_minimap
{
	mlx_image_t *img;
	mlx_image_t *display;
	mlx_image_t *img_player;
}	t_minimap;

typedef struct s_map
{
	mlx_image_t	*north;
	mlx_image_t	*south;
	mlx_image_t	*east;
	mlx_image_t	*west;
	t_color floor;
	t_color ceiling;
	char	**content;
	mlx_image_t	*img;
	int	width;
	int	height;
}			t_map;

typedef struct s_player
{
	t_coord plane;
	t_coord dir;
	t_coord pos;
	double height;
	double pitch;
	char orientation;
	bool sprint;
	bool jump;
	bool crouch;
	bool moving;
}	t_player;

typedef struct s_timer
{
	double gun;
	double music;
	double footstep;
	double breath;
}	t_timer;

typedef struct s_program
{
	mlx_t	*mlx;
	t_minimap minimap;
	t_map	map;
	t_player player;
	t_hud hud;
	char	*filename;
	int		fd;
	int		exit_value;
	t_timer timer;
	t_door door;
	enum mouse_mode cursor;
}			t_program;

#endif
