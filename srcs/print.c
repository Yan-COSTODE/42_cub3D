/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 21:49:27 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/15 21:49:27 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void print_fd_char(int fd, const char msg)
{
	write(fd, &msg, 1);
}

void print_fd(int fd, const char *msg)
{
	write(fd, msg, ft_strlen(msg));
}

int print_error(const char *msg)
{
	print_fd(2, "\x1b[1;31mError\n");
	print_fd(2, msg);
	print_fd(2, "\x1b[0m\n");
	return (EXIT_FAILURE);
}

int print_perror(const char *msg)
{
	print_fd(2, "\x1b[1;31mError\n");
	print_fd(2, msg);
	print_fd(2, ": ");
	perror("");
	print_fd(2, "\x1b[0m");
	return (EXIT_FAILURE);
}

int print_strerror(const char *msg, int error)
{
	print_fd(2, "\x1b[1;31mError\n");
	print_fd(2, msg);
	print_fd(2, ": ");
	print_fd(2, strerror(error));
	print_fd(2, "\x1b[0m\n");
	return (EXIT_FAILURE);
}

int print_mlxerror(const char *msg)
{
	print_fd(2, "\x1b[1;31mError\n");
	print_fd(2, msg);
	print_fd(2, ": ");
	print_fd(2, mlx_strerror(mlx_errno));
	print_fd(2, "\x1b[0m\n");
	return (EXIT_FAILURE);
}