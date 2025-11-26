/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:28:59 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/25 10:26:02 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*angle_op(const char **input)
{
	if (**input == '<')
	{
		(*input)++;
		if (**input == '<')
		{
			(*input)++;
			return (create_token(TOKEN_HEREDOC, "<<", 2));
		}
		return (create_token(TOKEN_REDIR_IN, "<", 1));
	}
	else if (**input == '>')
	{
		(*input)++;
		if (**input == '>')
		{
			(*input)++;
			return (create_token(TOKEN_APPEND, ">>", 2));
		}
		return (create_token(TOKEN_REDIR_OUT, ">", 1));
	}
	return (NULL);
}

char	*append_str(char *dest, const char *src, size_t len)
{
	size_t	old_len;
	char	*new_str;

	if (dest != NULL)
		old_len = ft_strlen(dest);
	else
		old_len = 0;
	new_str = malloc(old_len + len + 1);
	if (new_str == NULL)
		return (NULL);
	if (dest != NULL)
	{
		ft_memcpy(new_str, dest, old_len);
		free(dest);
	}
	ft_memcpy(new_str + old_len, src, len);
	new_str[old_len + len] = '\0';
	return (new_str);
}

static char	*append_plain(const char **input, char *words)
{
	const char	*start;

	start = *input;
	while (**input && !is_whitespace(**input)
		&& !is_op_start(**input) && **input != '\'' && **input != '"')
		(*input)++;
	if (*input > start)
	{
		words = append_str(words, start, *input - start);
		if (words == NULL)
			return (NULL);
	}
	return (words);
}

t_token	*extract_operator(const char **input)
{
	if (**input == '|')
	{
		(*input)++;
		return (create_token(TOKEN_PIPE, "|", 1));
	}
	else if (**input == '<' || **input == '>')
	{
		return (angle_op(input));
	}
	return (NULL);
}

t_token	*extract_words(const char **input)
{
	char	*words;
	t_token	*token;

	words = NULL;
	while (**input && !is_whitespace(**input) && !is_op_start(**input))
	{
		if (**input == '\'' || **input == '"')
		{
			words = append_quoted(input, words);
		}
		else
			words = append_plain(input, words);
		if (words == NULL)
			return (NULL);
	}
	token = create_token(TOKEN_WORD, words, ft_strlen(words));
	free(words);
	return (token);
}
