/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:38:22 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/26 10:38:22 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(char *dst, char *src)
{
	char	*tmp;

	if (!dst)
		dst = ft_strdup("");
	if (!src)
		src = ft_strdup("");
	tmp = ft_strjoin(dst, src);
	free(dst);
	return (tmp);
}

char	*remove_markers(const char *s)
{
	char	*clean;
	int		i;
	int		j;

	clean = malloc(ft_strlen(s) + 1);
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == S_QM_S || s[i] == S_QM_E
			|| s[i] == D_QM_S || s[i] == D_QM_E)
		{
			i++;
			continue ;
		}
		clean[j++] = s[i++];
	}
	clean[j] = '\0';
	return (clean);
}

void	free_words(char **words)
{
	int	i;

	i = 0;
	while (words && words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
}

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

int	process_expanded(char **words, char **dst, int idx, t_mode mode)
{
	int	i;

	i = 0;
	while (words && words[i])
	{
		if (mode == MODE_FILL)
		{
			dst[idx] = ft_strdup(words[i]);
			if (!dst[idx])
				return (-1);
		}
		idx++;
		i++;
	}
	return (idx);
}
