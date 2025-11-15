/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 08:22:02 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/15 08:44:42 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_until_esc_end(const char **input, char *words)
{
	const char	*start;

	start = *input;
	while (**input && **input != '"' && **input != '\\')
		(*input)++;
	if (*input > start)
	{
		words = append_str(words, start, *input - start);
		if (words == NULL)
			return (NULL);
	}
	return (words);
}

static char	*handle_esc_char(const char **input, char *words)
{
	char	c;

	if (**input == '\\' && *(*input + 1))
	{
		c = *(*input + 1);
		if (c == '"' || c == '\\')
			words = append_str(words, &c, 1);
		else
			words = append_str(words, *input, 2);
		if (words == NULL)
			return (NULL);
		*input += 2;
	}
	return (words);
}

static char	*handle_double_quote(const char **input, char *words)
{
	(*input)++;
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
	{
		free(words);
		return (NULL);
	}
	if (words == NULL)
	{
		words = append_str(NULL, "", 0);
		if (words == NULL)
			return (NULL);
	}
	(*input)++;
	return (words);
}

static char	*handle_single_quote(const char **input, char *words)
{
	const char	*start;

	(*input)++;
	start = *input;
	while (**input && **input != '\'')
		(*input)++;
	if (**input == '\0')
	{
		free(words);
		return (NULL);
	}
	words = append_str(words, start, *input - start);
	if (words == NULL)
		return (NULL);
	(*input)++;
	return (words);
}

char	*append_quoted(const char **input, char *words)
{
	if (**input == '"')
		return (handle_double_quote(input, words));
	else
		return (handle_single_quote(input, words));
}
