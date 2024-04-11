/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:16:33 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/11 15:18:55 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	*str;
	unsigned char	st;

	if (!s)
		return (NULL);
	str = (unsigned char *)s;
	st = (unsigned char)c;
	i = 0;
	while (str[i])
	{
		if (str[i] == st)
			return ((char *)&str[i]);
		i++;
	}
	if (st == 0)
		return ((char *)&str[i]);
	return (NULL);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	i;

	i = 0;
	src_len = ft_strlen(src);
	if (src_len < size)
	{
		while (i < src_len)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	else if (size != 0)
	{
		while (i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[size - 1] = '\0';
	}
	return (src_len);
}

void	ft_bzero(void *s, size_t n)
{
	size_t			*ptr;
	unsigned char	*ptr_last;

	ptr = (size_t *)s;
	while (n >= sizeof(size_t))
	{
		*ptr = 0;
		++ptr;
		n -= sizeof(size_t);
	}
	ptr_last = (unsigned char *)ptr;
	++n;
	while (--n)
	{
		*ptr_last = 0;
		++ptr_last;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;
	int				nb;

	if (nmemb * size > INT32_MAX || (nmemb > UINT16_MAX && size > UINT16_MAX))
		return (NULL);
	nb = nmemb * size;
	ptr = (unsigned char *)malloc(nb);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nb);
	return (ptr);
}

char	*ft_strnjoin(char *s1, char *s2, int n)
{
	char	*str;
	int		i;
	int		l;

	str = (char *)ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	l = ft_strlen(s1);
	if (s1)
	{
		while (s1[i])
		{
			str[i] = s1[i];
			i++;
		}
		free(s1);
	}
	while (s2[i - l] && i - l < n)
	{
		str[i] = s2[i - l];
		i++;
	}
	str[i] = '\0';
	return (str);
}
