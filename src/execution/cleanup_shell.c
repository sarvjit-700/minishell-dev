/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:53:54 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/28 08:36:15 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redir(t_redir *rdr)
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

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	int		i;

	while (cmd)
	{
		tmp_cmd = cmd->next;
		if (cmd->argv)
		{
			i = 0;
			while (i < cmd->argc)
			{
				free(cmd->argv[i]);
				i++;
			}
			free(cmd->argv);
		}
		free_redir(cmd->redir);
		free(cmd);
		cmd = tmp_cmd;
	}
}

void	free_tokens(t_token *token)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = next;
	}
}

void	cleanup_simple(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->cmd_list)
	{
		free_cmd_list(shell->cmd_list);
		shell->cmd_list = NULL;
	}
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
}

void	cleanup_shell(t_shell *shell)
{
	cleanup_simple(shell);
	if (shell->env_list)
	{
		free_env_list(shell->env_list);
		shell->env_list = NULL;
	}
	free(shell);
}
