/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 21:41:15 by ycostode          #+#    #+#             */
/*   Updated: 2024/03/15 21:41:15 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

size_t ft_strlen_split(char **s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		++i;
	return (i);
}

size_t ft_strlen(const char *s)
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
	int i;

	ptr = (char **)ft_calloc(ft_strlen_split(s) + 1, sizeof(char*));
	if (!ptr)
		return (NULL);
	i = -1;
	while (s[++i])
		ptr[i] = ft_strdup(s[i]);
	ptr[i] = NULL;
	return (ptr);
}

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

char	*ft_strchr_str(const char *s, const char *c)
{
	int				i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (ft_strcmp(&s[i], c) == 0)
			return ((char *)&s[i]);
		i++;
	}
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

char	*ft_strjoin(char *s1, char *s2)
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
	}
	while (s2[i - l])
	{
		str[i] = s2[i - l];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] || s2[i])
	{
		if ((s1[i] < s2[i]) || (s2[i] < s1[i]))
			return (s1[i] - s2[i]);
		else
			i++;
	}
	return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		len = 0;
	else if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	ptr = (char *)ft_calloc(len + 1, sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}