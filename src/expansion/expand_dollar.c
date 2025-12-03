/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 08:44:45 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/26 08:44:45 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*dup_env_value(t_env *env, const char *key)
{
	char	*val;

	val = get_env_value(env, key);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

static char	*expand_exit_code(int *i, char *res)
{
	char	*tmp;

	tmp = ft_itoa(g_exit_code);
	res = expand_var(res, tmp);
	free(tmp);
	*i += 2;
	return (res);
}

static char	*expand_digit(char *str, int *i, t_shell *shell, char *res)
{
	char	digit_var[2];
	char	*tmp;

	digit_var[0] = str[*i + 1];
	digit_var[1] = '\0';
	tmp = dup_env_value(shell->env_list, digit_var);
	res = expand_var(res, tmp);
	free(tmp);
	*i += 2;
	return (res);
}

static char	*expand_named_var(char *str, int *i, t_shell *shell, char *res)
{
	char	*tmp;
	char	*var;
	int		start;

	(*i)++;
	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	var = ft_substr(str, start, *i - start);
	tmp = dup_env_value(shell->env_list, var);
	res = expand_var(res, tmp);
	free(var);
	free(tmp);
	return (res);
}

char	*expand_dollar(char *str, int *i, t_shell *shell, char *res)
{
	if (str[*i + 1] == '?')
		return (expand_exit_code(i, res));
	if (ft_isdigit(str[*i + 1]))
		return (expand_digit(str, i, shell, res));
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		return (expand_named_var(str, i, shell, res));
	res = expand_var(res, "$");
	if (!res)
		return (NULL);
	(*i)++;
	return (res);
}
