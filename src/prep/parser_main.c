/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 10:13:36 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/15 10:13:36 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_parser_err(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	g_exit_code = 2;
	return (0);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_parser_state	ps;

	ps.token = tokens;
	ps.head = NULL;
	ps.tail = NULL;
	ps.curr = NULL;
	while (ps.token)
	{
		if (!process_token(&ps))
			return (free_cmd_list(ps.head), NULL);
		ps.token = ps.token->next;
	}
	if (ps.curr && ps.curr->argv)
		ps.curr->argv[ps.curr->argc] = NULL;
	return (ps.head);
}
