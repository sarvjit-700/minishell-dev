/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 08:41:25 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/15 08:41:25 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
