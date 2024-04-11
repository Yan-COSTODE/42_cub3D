/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 16:31:55 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 17:46:47 by ycostode         ###   ########.fr       */
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
}					t_door_status;

typedef struct s_coord_int
{
	int				x;
	int				y;
}					t_coord_int;

typedef struct s_door_elem
{
	t_coord_int		pos;
	double			status;
}					t_door_elem;

typedef struct s_door
{
	int				len;
	t_door_elem		*elem;
	mlx_image_t		*img;
}					t_door;

typedef struct s_coord
{
	double			x;
	double			y;
}					t_coord;

typedef struct s_color
{
	int				r;
	int				g;
	int				b;
	int				a;
	int64_t			rgba;
}					t_color;

typedef struct s_hud
{
	bool			shoot;
	int				gindex;
	double			bobbing;
	double			direction;
	mlx_image_t		*gun[4];
	mlx_image_t		*crosshair;
}					t_hud;

typedef struct s_minimap
{
	mlx_image_t		*img;
	mlx_image_t		*display;
	mlx_image_t		*img_player;
}					t_minimap;

typedef struct s_map
{
	int				width;
	int				height;
	t_color			floor;
	char			**content;
	t_color			ceiling;
	mlx_image_t		*img;
	mlx_image_t		*north;
	mlx_image_t		*east;
	mlx_image_t		*west;
	mlx_image_t		*south;
}					t_map;

typedef struct s_player
{
	bool			jump;
	t_coord			dir;
	t_coord			pos;
	bool			sprint;
	bool			crouch;
	bool			moving;
	t_coord			plane;
	double			pitch;
	double			height;
	char			orientation;
}					t_player;

typedef struct s_timer
{
	double			gun;
	double			music;
	double			breath;
	double			footstep;
}					t_timer;

typedef struct s_raycast
{
	int				x;
	int				y;
	int				hit;
	int				side;
	int				offset;
	int				draw_end;
	double			wall_x;
	t_coord_int		map;
	t_coord_int		tex;
	double			tex_pos;
	t_coord			ray_dir;
	double			camera_x;
	t_coord_int		step;
	int				draw_start;
	double			tex_step;
	int				line_height;
	mlx_image_t		*text;
	t_coord			side_dist;
	t_coord			delta_dist;
	double			current_dist;
	double			perp_wall_dist;
}					t_raycast;

typedef struct s_rotate
{
	int				x;
	int				y;
	double			angle;
	t_coord			x_rot;
	t_coord			y_rot;
	t_coord			trans;
	uint32_t		pixel;
	t_coord			rotated;
}					t_rotate;

typedef struct s_parse
{
	char			*tmp;
	char			*line;
	char			*final;
}					t_parse;

typedef struct s_calcul
{
	t_coord_int		x;
	t_coord_int		y;
	t_coord			diff;
	t_coord_int		comp;
	t_coord_int		index;
	t_coord_int		player;
}					t_calcul;

typedef struct s_program
{
	int				fd;
	mlx_t			*mlx;
	t_map			map;
	t_hud			hud;
	t_door			door;
	t_timer			timer;
	t_player		player;
	char			*filename;
	int				exit_value;
	t_minimap		minimap;
	enum mouse_mode	cursor;
}					t_program;

#endif
