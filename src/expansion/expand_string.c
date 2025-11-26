/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 09:11:40 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/26 09:11:40 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_char(char *dst, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (expand_var(dst, buf));
}

static char	*expand_squote(char *str, int *i, char *res)
{
	(*i)++;
	while (str[*i] && str[*i] != S_QM_E)
	{
		res = expand_char(res, str[*i]);
		if (!res)
			return (NULL);
		(*i)++;
	}
	if (str[*i] == S_QM_E)
		(*i)++;
	return (res);
}

static char	*expand_escape(char *str, int *i, char *res)
{
	(*i)++;
	if (str[*i])
	{
		res = expand_char(res, str[(*i)]);
		if (!res)
			return (NULL);
		(*i)++;
	}
	return (res);
}

char	*expand_string(char *str, t_shell *shell)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == S_QM_S)
			res = expand_squote(str, &i, res);
		else if (str[i] == '\\')
			res = expand_escape(str, &i, res);
		else if (str[i] == '$')
			res = expand_dollar(str, &i, shell, res);
		else
		{
			res = expand_char(res, str[i]);
			i++;
		}
		if (!res)
			return (NULL);
	}
	return (res);
}
