/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:19:27 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/28 15:14:46 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "MLX42/MLX42.h"
# include "struct.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>
# define BUFFER_SIZE 1024
# define FD_MAX 1024
# define HEIGHT 720
# define WIDTH 1280
# define MAX_RES 1024
# define MINIMAP_CELLS 10
# define MINIMAP_CELL 16
# define MINIMAP_SIZE MINIMAP_CELL * MINIMAP_CELLS
# define MINIMAP_OFFSET 16
# define MINIMAP_PLAYER 8
# define CROUCH_SPEED 2.0
# define MOVE_SPEED 6.0
# define SPRINT_SPEED 10.0
# define ROTATE_SPEED 4.5
# define PITCH_SPEED 1500.0
# define PITCH_LIMIT 200
# define JUMP_FORCE 1100.0
# define JUMP_MAX 380
# define GRAVITY_FORCE 1100.0
# define CROUCH_FORCE 500.0
# define CROUCH_MAX -200
# define CROSSHAIR 32
# define ANIM_DELAY 0.05
# define MAX_BOBBING 50
# define BIG 1e30
# define MUSIC_LENGTH 3 * 60 + 28

// LIBFT
size_t ft_strlen(const char *s);
size_t ft_strlen_split(char **s);
char	*ft_strchr(const char *s, int c);
char	*ft_strchr_str(const char *s, const char *c);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strnjoin(char *s1, char *s2, int n);
char	**ft_freesplit(char **split);
int	ft_strcmp(const char *s1, const char *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**ft_split(char const *s, char c);
bool	atoi_convert(const char *s, unsigned char *n);
char	*ft_strdup(const char *s);

// PRINT
void print_fd_char(int fd, const char msg);
void print_fd(int fd, const char *msg);
int print_error(const char *msg);
int print_perror(const char *msg);
int print_strerror(const char *msg, int error);
int print_mlxerror(const char *msg);

// PARSING
void parse(t_program *program);
int	parsing(t_program *program);
char	*get_next_line(int fd);
uint32_t get_color_rgba(int r, int g, int b, int a);

// MAP
char	get_at(const t_map map, int x, int y);
void	set_at(t_map *map, int x, int y, char c);
void	print_map(t_map map);

// MOVE
void move(t_program *program);
void rotate(t_program *program);

// TEST
void setup_shoot(t_program *program);
void shoot(t_program *program, bool try);
void draw(t_program *program);
void draw_minimap(t_program *program);
void show_shoot_frame(t_program *program);

// SOUND
void play_background(t_program *program);
void play_walk(t_program *program);
void play_gun();
void play_jump();
void play_land();

#endif
