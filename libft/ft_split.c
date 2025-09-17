/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:48:52 by ssukhija          #+#    #+#             */
/*   Updated: 2025/07/29 13:10:24 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while ((s[i] != '\0') && ((s[i] == c)))
		i++;
	while (s[i] != '\0')
	{
		if ((s[i] != '\0') && (!(s[i] == c)))
		{
			while ((s[i] != '\0') && (!(s[i] == c)))
				i++;
			count++;
		}
		else
			i++;
	}
	return (count);
}

static void	free_split_partial(char **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*alloc_word(char *start, int len)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(len + 1);
	if (word == NULL)
		return (NULL);
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static int	move(char *str, char c, int i, int x)
{
	if (x == 1)
	{
		while (str[i] == c)
			i++;
	}
	else if (x == 2)
	{
		while (str[i] && str[i] != c)
			i++;
	}
	return (i);
}

char	**ft_split(char *str, char c)
{
	char	**arr;
	int		i;
	int		j;
	int		start;

	i = 0;
	j = 0;
	arr = malloc(sizeof(char *) * (count_words(str, c) + 1));
	if (arr == NULL)
		return (0);
	while (str[i])
	{
		i = move(str, c, i, 1);
		start = i;
		i = move(str, c, i, 2);
		if (i > start)
		{
			arr[j] = alloc_word(&str[start], i - start);
			if (arr[j] == NULL)
				return (free_split_partial(arr, j), NULL);
			j++;
		}
	}
	arr[j] = NULL;
	return (arr);
}
