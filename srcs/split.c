/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycostode <ycostode@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 20:10:12 by ycostode          #+#    #+#             */
/*   Updated: 2024/04/09 19:55:41 by ycostode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**ft_freesplit(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return (NULL);
	while (split[i])
		free(split[i++]);
	free(split);
	return (NULL);
}

static int	ft_strcount(const char *str, char c)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (str[i])
	{
		if (i == 0 && str[i] != c)
			++n;
		else if (str[i] == c && str[i + 1] != c && str[i + 1])
			++n;
		++i;
	}
	return (n + 1);
}

static char	*ft_strtruncate(const char *s, int start, int index)
{
	char	*str;

	str = ft_substr(s, start, index - start + 1);
	if (!str)
		return (NULL);
	return (str);
}

static char	**ft_split_exec(char **str, const char *s, char c)
{
	int	i;
	int	j;
	int	start;

	i = -1;
	j = 0;
	start = 0;
	while (s[++i])
	{
		if (s[i] == c && i != 0 && s[i - 1] != c)
			str[j++] = ft_strtruncate(s, start, i - 1);
		else if (!s[i + 1] && s[i] != c)
			str[j++] = ft_strtruncate(s, start, i);
		if (s[i] == c && s[i + 1] != c)
			start = i + 1;
		else if (i == 0 && s[i] != c)
			start = i;
		if (j != 0 && str[j - 1] == NULL)
			return (ft_freesplit(str));
	}
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;

	if (!s)
		return (NULL);
	str = (char **)ft_calloc(ft_strcount(s, c), sizeof(char *));
	if (!str)
		return (NULL);
	return (ft_split_exec(str, s, c));
}
