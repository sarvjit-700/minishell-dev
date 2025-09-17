/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:28:59 by ssukhija          #+#    #+#             */
/*   Updated: 2025/08/09 00:59:10 by ssukhija         ###   ########.fr       */
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

static t_token	*extract_quoted_string(const char **input)
{
	char		quote;
	const char	*start;
	size_t		len;

	quote = **input;
	(*input)++;
	start = *input;
	while (**input != '\0' && **input != quote)
		(*input)++;
	if (**input != quote)
		return (NULL);
	len = *input - start;
	(*input)++;
	return (create_token(TOKEN_WORD, start, len));
}

static t_token	*extract_word(const char **input)
{
	const char	*start;

	start = *input;
	while (**input != '\0' && !is_whitespace(**input) && !is_op_start(**input))
		(*input)++;
	return (create_token(TOKEN_WORD, start, *input - start));
}

t_token	*tokenize(const char **input)
{
	t_token	*head;
	t_token	*new_token;

	head = NULL;
	while (**input != '\0')
	{
		if (is_whitespace(**input))
		{
			(*input)++;
			continue ;
		}
		else if (is_op_start(**input))
			new_token = extract_operator(input);
		else if (**input == '\'' || **input == '"')
			new_token = extract_quoted_string(input);
		else
			new_token = extract_word(input);
		if (new_token)
			add_token(&head, new_token);
		else
			break ;
	}
	return (head);
}
