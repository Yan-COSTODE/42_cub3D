/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 21:41:15 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 18:12:46 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

size_t	ft_strlen_split(char **s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		++i;
	return (i);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		++i;
	return (i);
}

bool	atoi_convert(const char *s, unsigned char *n)
{
	int		i;
	long	tmp;

	i = 0;
	*n = 0;
	tmp = 0;
	if (s[i] == '-')
		return (false);
	if (s[i] == '+')
		++i;
	while (s[i] == '0')
		++i;
	if (ft_strlen(&s[i]) > 10 || (ft_strlen(&s[i]) == 0 && s[i - 1] != '0'))
		return (false);
	while (s[i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (false);
		tmp = tmp * 10 + (s[i] - '0');
		++i;
	}
	if (tmp > UINT8_MAX)
		return (false);
	*n = tmp;
	return (true);
}

char	*ft_strdup(const char *s)
{
	char	*ptr;

	ptr = (char *)ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s, ft_strlen(s) + 1);
	return (ptr);
}

char	**ft_strdup_split(char **s)
{
	char	**ptr;
	int		i;

	ptr = (char **)ft_calloc(ft_strlen_split(s) + 1, sizeof(char *));
	if (!ptr)
		return (NULL);
	i = -1;
	while (s[++i])
		ptr[i] = ft_strdup(s[i]);
	ptr[i] = NULL;
	return (ptr);
}
