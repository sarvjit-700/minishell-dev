/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:28:59 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/03 10:40:29 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token	*op_angle(const char **input)
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
	printf("\nDEBUG: [ex_op] found op '%c'\n", **input);
	if (**input == '|')
	{
		(*input)++;
		return (create_token(TOKEN_PIPE, "|", 1));
	}
	else if (**input == '<' || **input == '>')
	{
		return (op_angle(input));
	}
	return (NULL);
}

static char *append_str(char *dest, const char *src, size_t len)
{
	size_t old_len;
	char	*new_str;

	printf("\nDEBUG: [app_str] words already stored : '%s'\n", dest);
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
	printf("\nDEBUG:[app_str] storing -> '%s'\n", new_str);
	new_str[old_len + len] = '\0';
	return (new_str);	
}


static char	*append_until_esc_end(const char **input, char *words)
{
	const char *start;
	
	start = *input;
	printf("\nDEBUG: [app until esc](bksl) or quo ->\n");
	while (**input && **input != '"' && **input != '\\')
	{
		(*input)++;
		printf("\nDEBUG: [app_until] moved to : '%c'", **input);
	}
	printf("\nDEBUG: [app_until] found : '%c' at %s\n", **input, *input);
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
	printf("\nDEBUG: [handle esc] -> : '%c'\n", **input);
	printf("DEBUG: check if now bksl AND NEXT is quo/bksl from '%c'\n", **input);
	if (**input == '\\' && (*(*input + 1) == '"' || *(*input + 1) == '\\'))
	{
		printf("DEBUG: NEXT is '%c' so append\n", *(*input + 1));
		words = append_str(words, *input + 1, 1);
		if (words == NULL)
			return (NULL);
		*input += 2;
		printf("DEBUG: [handle esc] moved 2, now at '%c' from '%s'\n", **input, *input);
	}
	return (words);
}

static char	*handle_double_quote(const char **input, char *words)
{
	printf("\nDEBUG : [hdle_dble_quo] from current '%c' move to '%c'\n", **input, *(*input + 1));
	(*input)++;
	while (**input && **input != '"')
	{
		if (**input == '\\' && (*(*input + 1) == '"' || *(*input + 1) == '\\'))
		{
			printf("DEBUG : [hdq] found esc char '%c' go to [handle_esc]\n", **input);
			words = handle_esc_char(input, words);
			if (words == NULL)
				return (NULL);
		}
		else
		{
			printf("DEBUG : [hdq] go to [app_until_esc_end]\n");
			words = append_until_esc_end(input,words);
			if (words == NULL)
				return (NULL);
		}

	}
	printf("DEBUG : [hdq] FOUND quo or end \n");
	printf("DEBUG : [hdq] INPUT NOW -> %c at %s (blank spaces mean end)\n", **input, *input);
	if (**input != '"')
	{
		printf("\nDEBUG: [hdq] unclosed quote ERROR - return NULL\n");
		return (NULL);
	}
	if (words == NULL)
	{
		words = append_str(NULL, "", 0);
		if (words == NULL)
			return (NULL);
	}
	(*input)++;
	printf("DEBUG : Leaving [hdq] moved forward, \ngo back to [ext_wrds] with %d from %s (0 means end)", **input, *input);
	return (words);
}

static char	*handle_single_quote(const char **input, char *words)
{
	const char	*start;

	start = *input;
	printf("\nDEBUG: [HSQ]\n");
	while (**input && **input != '\'')
		(*input)++;
	if (**input == '\0')
		return (NULL);
	words = append_str(words, start, *input - start);
	if (words == NULL)
		return (NULL);
	(*input)++;
	return (words);	
}

static char	*append_quoted(const char **input, char *words)
{
	printf("\nDEBUG: [app_quo] quo type => '%c'\n Remaining input [%s]\n Already stored [%s]\n", **input, *input, words);
    if (**input == '"')
        return handle_double_quote(input, words);
    else // single quote
        return handle_single_quote(input, words);
}


static char	*append_plain(const char **input, char *words)
{
	const char	*start;
	
	start = *input;
	printf("\nDEBUG: [app_pln] going until spc/op/esc/quo\n");
	while (**input && !is_whitespace(**input) && \
		!is_op_start(**input) && **input != '\'' && **input != '"')
	{
		(*input)++;
		printf("DEBUG: [app_pln] looking for end/bks/quo : '%c' at [%s]\n", **input, *input);
	}
		
	printf("\nDEBUG: [app_pln] found : '%d' (32=space 0=end 34=quo 124=pipe) before [%s]\n Now store/append all until here. \n", **input, *input);
	if (*input > start)
	{
		words = append_str(words, start, *input - start);
		printf("\nDEBUG: [app_pln] after append : '%s'\n", words);
		if (words == NULL)
			return (NULL);
	}
	printf("DEBUG : Leaving [app_pln] go back to [ext_wrds] with [%c] from [%s]", **input, *input);
	return (words);
}


static t_token *extract_words(const char **input)
{
	char	*words;
	
	words = NULL;
	while (**input && !is_whitespace(**input) && !is_op_start(**input))
	{
		printf("\nDEBUG: [ext_wrds] in loop until end/spc/op\n**INPUT now ---> %s \n", *input);
		if (**input == '\'' || **input == '"') // '\'' means ' \is an escape
			words = append_quoted(input, words);
		else
			words = append_plain(input, words);
		if (words == NULL)
			return (NULL);
	}
	printf("\nDEBUG: [ext_wrds] create token\n");
	return create_token(TOKEN_WORD, words, ft_strlen(words));
}


t_token	*tokenize(const char **input)
{
	t_token	*head;
	t_token	*new_token;

	head = NULL;
	new_token = NULL;
	printf("TESTING : --> '%s'\n", *input);
	while (**input != '\0')
	{
		printf("DEBUG: [tokenize] now at char '%c'\n", **input);
		if (is_whitespace(**input))
		{
			printf("-- space found, move on --  \n");
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
