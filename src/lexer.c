/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:28:59 by ssukhija          #+#    #+#             */
/*   Updated: 2025/09/21 22:45:21 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token	*sym_angle(const char **input)
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

static t_token	*extract_operator(const char **input)
{
	if (**input == '|')
	{
		(*input)++;
		return (create_token(TOKEN_PIPE, "|", 1));
	}
	else if (**input == '<' || **input == '>')
	{
		return (sym_angle(input));
	}
	return (NULL);
}

static char *append_str(char *dest, const char *src, size_t len)
{
	size_t old_len;
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

static char *append_or_fail(char *words, const char *start, size_t len)
{
    char *res;
	
	res = append_str(words, start, len);
    if (res == NULL)
        return (NULL);
    return (res);
}


static char	*handle_double_quote(const char **input, char *words)
{
	const char	*start;

	start = *input;
	while (**input && **input != '"')
	{
		if (**input == '\\' &&(*(*input + 1) == '"' || *(*input + 1) == '\\'))
		{
			if (*input > start)
				words = append_or_fail(words, start, *input - start);
			words = append_or_fail(words, *input + 1, 1);
			*input += 2;
			start = *input;
			continue ;
		}
		(*input)++;
	}
	if (**input == '\0')
		return (NULL);
	if (*input > start)
		words = append_or_fail(words, start, *input - start);
	(*input)++;
	return (words);
}

static char	*handle_single_quote(const char **input, char *words)
{
	const char	*start;

	start = *input;
	while (**input && **input != '\'')
		(*input)++;
	if (**input == '\0')
		return (NULL);
	words = append_or_fail(words, start, *input - start);
	(*input)++;
	return (words);	
}

static char	*append_quoted(const char **input, char *words)
{
    if (**input == '"')
        return handle_double_quote(input, words);
    else // single quote
        return handle_single_quote(input, words);
}


static char	*append_plain(const char **input, char *words)
{
	const char	*start;
	
	start = *input;
	while (**input && !is_whitespace(**input) && \
		!is_op_start(**input) && **input != '\'' && **input != '"')
		(*input)++;
	if (*input > start)
		words = append_or_fail(words, start, *input - start);
	return (words);
}


static t_token *extract_words(const char **input)
{
	char	*words;
	
	words = NULL;
	while (**input && !is_whitespace(**input) && !is_op_start(**input))
	{
		if (**input == '\'' || **input == '"') // '\'' means ' \is an escape
			words = append_quoted(input, words);
		else
			words = append_plain(input, words);
		if (words == NULL)
			return (NULL);
	}
	return create_token(TOKEN_WORD, words, ft_strlen(words));
}

// static t_token	*extract_word(const char **input)
// {
// 	const char	*start;

// 	start = *input;
// 	while (**input != '\0' && !is_whitespace(**input) && !is_op_start(**input))
// 		(*input)++;
// 	return (create_token(TOKEN_WORD, start, *input - start));
// }

t_token	*tokenize(const char **input)
{
	t_token	*head;
	t_token	*new_token;

	head = NULL;
	new_token = NULL;
	while (**input != '\0')
	{
		//printf("DEBUG: at char '%c'\n", **input);
		if (is_whitespace(**input))
		{
			(*input)++;
			continue ;
		}
		else if (is_op_start(**input))
			new_token = extract_operator(input);
		else
			new_token = extract_words(input);
		if (new_token)
			add_token(&head, new_token);
		else
			break ;
	}
	return (head);
}
