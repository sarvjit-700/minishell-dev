/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:28:59 by ssukhija          #+#    #+#             */
/*   Updated: 2025/09/22 12:50:55 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static t_token	*extract_operator(const char **input)
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


static char	*append_until_esc_end(const char **input, char *words)
{
	const char *start;
	
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

// handle \" and \\ only
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
			words = append_until_esc_end(input,words);
		if (words == NULL)
				return (NULL);
	}
	if (**input != '"')
	{
		free(words);   // clean up anything we built so far
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
	{
		words = append_str(words, start, *input - start);
		if (words == NULL)
			return (NULL);
	}
	return (words);
}


static t_token *extract_words(const char **input)
{
	char	*words;
	t_token	*token;
	
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
	token = create_token(TOKEN_WORD, words, ft_strlen(words));
	free(words);
	return (token);
}


t_token	*tokenize(const char **input)
{
	t_token	*head;
	t_token	*new_token;

	head = NULL;
	new_token = NULL;
	while (**input != '\0')
	{
		if (is_whitespace(**input))
		{
			(*input)++;
			continue ;
		}
		else if (is_op_start(**input))
			new_token = extract_operator(input);
		else
			new_token = extract_words(input);
		if (new_token == NULL)
		{
			free_tokens(head);
			return (NULL);
		} 
		add_token(&head, new_token);
	}
	return (head);
}