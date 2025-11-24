/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:56:52 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/20 16:47:07 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *append_char(char *dst, char c)
{
    char	buf[1];
	
    buf[0] = c;
    return (append_str(dst, buf, 1));
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
	token->next = NULL;
	token->value = malloc(len + 1);
	if (token->value == NULL)
	{
		free(token);
		return (NULL);
	}
	ft_memcpy(token->value, start, len);
	token->value[len] = '\0';
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}
