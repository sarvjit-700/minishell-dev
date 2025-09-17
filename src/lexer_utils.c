/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:56:52 by ssukhija          #+#    #+#             */
/*   Updated: 2025/08/09 00:59:19 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*copy;

	copy = malloc(sizeof(char) * (n + 1));
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s, n);
	copy[n] = '\0';
	return (copy);
}

bool	is_whitespace(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

bool	is_op_start(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*create_token(t_token_type type, const char *start, size_t len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strndup(start, len); // creates a copy of the token's text
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}
