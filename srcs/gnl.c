/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 19:21:40 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 17:54:19 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	ft_join(char *buff, char **s)
{
	char	*ref;

	if (buff[0] == '\0')
	{
		*s = NULL;
		return ;
	}
	ref = ft_strchr(buff, '\n');
	if (ref)
	{
		*s = ft_strnjoin(*s, buff, ref - buff + 1);
		ft_strlcpy(buff, ref + 1, BUFFER_SIZE);
	}
	else
		*s = ft_strnjoin(*s, buff, BUFFER_SIZE);
}

static char	*ft_get_line(int fd)
{
	static char	buff[BUFFER_SIZE + 1];
	int			bytes;
	char		*s;

	bytes = 1;
	s = NULL;
	ft_join(buff, &s);
	while (bytes > 0 && !ft_strchr(s, '\n'))
	{
		bytes = read(fd, buff, BUFFER_SIZE);
		if (bytes < BUFFER_SIZE)
			buff[bytes] = '\0';
		if (bytes > 0)
			ft_join(buff, &s);
	}
	if (s && s[0] == '\0')
		free(s);
	return (s);
}

char	*get_next_line(int fd)
{
	char	*s;

	if (fd < 0 || fd > FD_MAX)
		return (NULL);
	s = ft_get_line(fd);
	return (s);
}
