/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 00:55:32 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/13 10:28:22 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redir(t_redir *rdr)
{
	t_redir	*tmp;

	while (rdr)
	{
		tmp = rdr->next;
		free(rdr->filename);
		free(rdr);
		rdr = tmp;
	}
}

static t_cmd	*new_command(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (new_cmd == NULL)
		return (NULL);
	new_cmd->argv = NULL;
	new_cmd->argc = 0;
	new_cmd->redir = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

static int	add_arg(t_cmd *cmd, const char *arg)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (cmd->argc + 2));
	if (new_argv == NULL)
		return (0);
	i = 0;
	while (i < cmd->argc)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(arg);
	if (new_argv[i] == NULL)
	{
		free(new_argv);
		return (0);
	}
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	cmd->argc++;
	return (1);
}

static int	add_redir(t_cmd *cmd, t_token_type type, const char *filename)
{
	t_redir	*new_rdr;
	t_redir	*tmp;

	new_rdr = malloc(sizeof(t_redir));
	if (new_rdr == NULL)
		return (0);
	new_rdr->type = type;
	new_rdr->filename = ft_strdup(filename);
	if (new_rdr->filename == NULL)
	{
		free(new_rdr);
		return (0);
	}
	new_rdr->next = NULL;
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

static int	start_cmd(t_parser_state *ps)
{
	if (ps->curr)
		return (1);
	ps->curr = new_command();
	if (ps->curr == NULL)
		return (0);
	if (ps->head == NULL)
	{
		ps->tail = ps->curr;
		ps->head = ps->tail;
	}
	else
	{
		ps->tail->next = ps->curr;
		ps->tail = ps->curr;
	}
	return (1);
}

static int	handle_word(t_parser_state *ps)
{
	if (!start_cmd(ps))
		return (0);
	if (!add_arg(ps->curr, ps->token->value))
		return (0);
	return (1);
}

/*

static int	handle_string(t_parser_state *ps)
{
    if (!start_cmd(ps))
        return (0);
    // Optional: store a "quoted" flag somewhere
    if (!add_arg(ps->curr, ps->token->value))
        return (0);
    return (1);
}
    */
static int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static int	handle_parser_err(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	g_exit_code = 2;
	return (0);
}

static int	handle_pipe(t_parser_state *ps)
{
	const char *msg = "minishell: syntax error near unexpected token `|'\n";
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
	t_token	*file_token;
	const char *msg = "minishell: syntax error near unexpected token `newline'\n";

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

static int	process_token(t_parser_state *ps)
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
