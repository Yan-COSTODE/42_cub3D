/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 16:31:55 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 20:06:42 by ycostode         ###   ########.fr       */
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
	t_door_elem		*elem;
	int				len;
	mlx_image_t		*img;
}					t_door;

typedef struct s_coord
{
	double			x;
	double			y;
}					t_coord;

typedef struct s_color
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
	uint32_t		rgba;
}					t_color;

typedef struct s_hud
{
	mlx_image_t		*crosshair;
	mlx_image_t		*gun[4];
	int				gindex;
	double			bobbing;
	double			direction;
	bool			shoot;
}					t_hud;

typedef struct s_minimap
{
	mlx_image_t		*img;
	mlx_image_t		*display;
	mlx_image_t		*img_player;
}					t_minimap;

typedef struct s_map
{
	mlx_image_t		*north;
	mlx_image_t		*south;
	mlx_image_t		*east;
	mlx_image_t		*west;
	t_color			floor;
	t_color			ceiling;
	char			**content;
	mlx_image_t		*img;
	int				width;
	int				height;
}					t_map;

typedef struct s_player
{
	t_coord			plane;
	t_coord			dir;
	t_coord			pos;
	double			height;
	double			pitch;
	char			orientation;
	bool			sprint;
	bool			jump;
	bool			crouch;
	bool			moving;
}					t_player;

typedef struct s_timer
{
	double			gun;
	double			music;
	double			footstep;
	double			breath;
}					t_timer;

typedef struct s_raycast
{
	int				x;
	t_coord			ray_dir;
	double			camera_x;
	t_coord_int		map;
	t_coord			side_dist;
	t_coord			delta_dist;
	double			perp_wall_dist;
	t_coord_int		step;
	int				hit;
	int				side;
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				offset;
	mlx_image_t		*text;
	double			wall_x;
	t_coord_int		tex;
	double			tex_step;
	double			tex_pos;
	int				y;
	double			current_dist;
}					t_raycast;

typedef struct s_rotate
{
	double			angle;
	t_coord			x_rot;
	t_coord			y_rot;
	int				x;
	int				y;
	t_coord			trans;
	t_coord			rotated;
	uint32_t		pixel;
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
	t_coord_int		player;
	t_coord_int		index;
	t_coord_int		comp;
}					t_calcul;

typedef struct s_program
{
	mlx_t			*mlx;
	t_minimap		minimap;
	t_map			map;
	t_player		player;
	t_hud			hud;
	char			*filename;
	int				fd;
	int				exit_value;
	t_timer			timer;
	t_door			door;
	enum mouse_mode	cursor;
}					t_program;

#endif
