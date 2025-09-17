t_cmd	*parse_tokens(t_token *tokens)
{
	parse_init();
	while (ps.token)
	{
		if (ps.token->type == TOKEN_WORD)
		{
			if (!handle_word(&ps))
				return (free_cmd_list(ps.head), NULL);
		}
		else if (ps.token->type == TOKEN_PIPE)
		{
			if (!handle_pipe(&ps))
				return (free_cmd_list(ps.head), NULL);
		}
		else if (ps.token->type == TOKEN_REDIR_IN
			|| ps.token->type == TOKEN_REDIR_OUT
			|| ps.token->type == TOKEN_APPEND 
			|| ps.token->type == TOKEN_HEREDOC)
		{
			if (!handle_redir(&ps))
				return (free_cmd_list(ps.head), NULL);
		}
		else
			return (free_cmd_list(ps.head), NULL);
		ps.token = ps.token->next;
	}
	return (ps.head);
}