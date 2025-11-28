/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:00:34 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/27 10:00:34 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_until_esc_end(const char **input, char *words)
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

char	*handle_esc_char(const char **input, char *words)
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
