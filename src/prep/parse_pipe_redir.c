/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe_redir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 00:55:32 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/18 16:07:58 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_redir(t_cmd *cmd, t_token_type type, const char *filename)
{
	t_redir	*new_rdr;
	t_redir	*tmp;

	new_rdr = malloc(sizeof(t_redir));
	if (new_rdr == NULL)
		return (0);
	memset(new_rdr, 0, sizeof(t_redir));
	new_rdr->type = type;
	new_rdr->fd = -1;
	new_rdr->filename = ft_strdup(filename);
	if (new_rdr->filename == NULL)
	{
		free(new_rdr);
		return (0);
	}
	if (cmd->redir == NULL)
		cmd->redir = new_rdr;
	else
	{
		tmp = cmd->redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_rdr;
	}
	return (1);
}

static int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static int	handle_pipe(t_parser_state *ps)
{
	const char	*msg;

	msg = "minishell: syntax error near unexpected token `|'\n";
	if (ps->curr == NULL || ps->token->next == NULL
		|| ps->token->next->type == TOKEN_PIPE
		|| is_redir_token(ps->token->next->type))
	{
		return (handle_parser_err(msg));
	}
	ps->curr = NULL;
	return (1);
}

static int	handle_redir(t_parser_state *ps)
{
	t_token		*file_token;
	const char	*msg;

	msg = "minishell: syntax error near unexpected token `newline'\n";
	if (!start_cmd(ps))
		return (handle_parser_err(msg));
	file_token = ps->token->next;
	if (file_token == NULL || file_token->type != TOKEN_WORD)
		return (handle_parser_err(msg));
	if (!add_redir(ps->curr, ps->token->type, file_token->value))
		return (handle_parser_err(msg));
	ps->token = file_token;
	return (1);
}

int	process_token(t_parser_state *ps)
{
	if (ps->token->type == TOKEN_WORD)
		return (handle_word(ps));
	else if (ps->token->type == TOKEN_PIPE)
		return (handle_pipe(ps));
	else if (is_redir_token(ps->token->type))
		return (handle_redir(ps));
	else
		return (0);
}
