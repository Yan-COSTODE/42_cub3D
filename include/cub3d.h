/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:19:27 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:34:32 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "MLX42/MLX42.h"
# include "struct.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# define BUFFER_SIZE 1024
# define FD_MAX 1024
# define HEIGHT 720
# define WIDTH 1280
# define MAX_RES 1024
# define MINIMAP_CELLS 10
# define MINIMAP_CELL 16
# define MINIMAP_SIZE 160 // MINIMAP_CELL * MINIMAP_CELLS
# define MINIMAP_OFFSET 16
# define MINIMAP_PLAYER 8
# define CROUCH_SPEED 1.5
# define MOVE_SPEED 3.0
# define SPRINT_SPEED 5.0
# define ROTATE_SPEED 2.0
# define PITCH_SPEED 1000.0
# define PITCH_LIMIT 300
# define JUMP_FORCE 1100.0
# define JUMP_MAX 350
# define GRAVITY_FORCE 1100.0
# define CROUCH_FORCE 500.0
# define CROUCH_MAX -200
# define CROSSHAIR 32
# define ANIM_DELAY 0.05
# define MAX_BOBBING 50
# define BIG 1e30
# define MUSIC_LENGTH 208.0 // 3 * 60 + 28
# define RED_MASK 0xFF000000
# define GREEN_MASK 0x00FF0000
# define BLUE_MASK 0x0000FF00
# define MAX_FOG 1
# define FOG_LENGTH 3
# define MOVE_OFFSET 1.4
# define MOUSE_SENS 15
# define DOOR_OPEN 4.0
# define DOOR_CLOSE 4.0

// LIBFT
char		*ft_strdup(const char *s);
size_t		ft_strlen(const char *s);
size_t		ft_strlen_split(char **s);
void		ft_bzero(void *s, size_t n);
char		**ft_strdup_split(char **s);
char		**ft_freesplit(char **split);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strchr(const char *s, int c);
char		**ft_split(char const *s, char c);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strnjoin(char *s1, char *s2, int n);
int			ft_strcmp(const char *s1, const char *s2);
bool		atoi_convert(const char *s, unsigned char *n);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_substr(char const *s, unsigned int start, size_t len);

// PRINT
int			print_error(const char *msg);
int			print_perror(const char *msg);
int			print_mlxerror(const char *msg);
void		print_fd(int fd, const char *msg);

// PARSING
char		*get_next_line(int fd);
void		parse(t_program *program);
int			parsing(t_program *program);
void		open_file(t_program *program);
void		fill_width(t_program *program);
void		parse_door(t_program *program);
void		close_file(t_program *program);
void		parse_player(t_program *program);
int			check_borders(t_program *program);
void		parse_content(t_program *program);
void		parse_error(int *status, char *name);
uint32_t	get_color_rgba(int r, int g, int b, int a);
void		flood_fill(t_program *program, char **map, int x, int y);
int			parsing_utils(t_program *program, t_coord_int *i, int *player);
int			is_space_surrounded(t_program *program, char **map, int y, int x);
void		parse_error_color(int *status, char *name, char **split,
				char *color);

// MAP
void		draw(t_program *program);
void		draw_minimap(t_program *program);
void		rotate_minimap(t_program *program);
char		get_at(const t_map map, int x, int y);
void		set_at(t_map *map, int x, int y, char c);

// MOVE
void		jump(t_program *program);
void		move(t_program *program);
void		crouch(t_program *program);
void		sprint(t_program *program);
void		rotate(t_program *program);

// LOOP
void		update(void *param);
int			start(t_program *program);
void		on_destroy(t_program *program);

// UTILS
void		img_setup(t_program *program);
void		init_program(t_program *program);
void		change_cursor(mouse_key_t button, action_t action,
				modifier_key_t mods, void *param);

// SHOOT
void		setup_shoot(t_program *program);
void		shoot(t_program *program, bool try);
void		show_shoot_frame(t_program *program);

// RAYCAST
uint32_t	set_fog(uint32_t original, double scale);
void		raycast_1(t_raycast *raycast, t_program *program);
void		raycast_2(t_raycast *raycast, t_program *program);
void		raycast_3(t_raycast *raycast, t_program *program);
void		raycast_4(t_raycast *raycast, t_program *program);
void		raycast_5(t_raycast *raycast, t_program *program);
void		raycast_6(t_raycast *raycast, t_program *program);
void		raycast_7(t_raycast *raycast, t_program *program);
void		raycast_8(t_raycast *raycast, t_program *program);
void		raycast_3bis(t_raycast *raycast, t_program *program);
void		raycast_5bis(t_raycast *raycast, t_program *program);
void		raycast_7bis(t_raycast *raycast, t_program *program);

// DOOR
void		update_door(t_program *program);
void		set_door(t_program *program, t_coord_int coord);
double		get_door(t_program *program, t_coord_int coord);

// SOUND
void		play_gun(void);
void		play_jump(void);
void		play_land(void);
void		play_door(void);
void		play_door_boom(void);
void		update_sound(t_program *program);

#endif
