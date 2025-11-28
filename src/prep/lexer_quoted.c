/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 08:22:02 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/27 10:07:40 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*quote_err(char *words)
{
	g_exit_code = 2;
	write(2, "minishell: syntax error: unclosed quote\n", 41);
	free(words);
	return (NULL);
}

static char	*close_quote_segment(const char **input, char *words, char end_char)
{
	if (!words)
		return (NULL);
	words = append_char(words, end_char);
	if (!words)
		return (NULL);
	(*input)++;
	return (words);
}

static char	*handle_double_quote(const char **input, char *words)
{
	(*input)++;
	words = append_char(words, D_QM_S);
	if (!words)
		return (NULL);
	while (**input && **input != '"')
	{
		if (**input == '\\')
			words = handle_esc_char(input, words);
		else
			words = append_until_esc_end(input, words);
		if (words == NULL)
			return (NULL);
	}
	if (**input != '"')
		return (quote_err(words));
	return (close_quote_segment(input, words, D_QM_E));
}

static char	*handle_single_quote(const char **input, char *words)
{
	const char	*start;

	(*input)++;
	words = append_char(words, S_QM_S);
	if (!words)
		return (NULL);
	start = *input;
	while (**input && **input != '\'')
		(*input)++;
	if (**input == '\0')
		return (quote_err(words));
	words = append_str(words, start, *input - start);
	if (words == NULL)
		return (NULL);
	return (close_quote_segment(input, words, S_QM_E));
}

char	*append_quoted(const char **input, char *words)
{
	if (**input == '"')
		return (handle_double_quote(input, words));
	else
		return (handle_single_quote(input, words));
}
