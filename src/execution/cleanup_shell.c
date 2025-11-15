/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:53:54 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/05 11:53:54 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int extract_exit_code(int status)
{
    int sig;
    
    sig = status & 0x7F;
    if (sig == 0)
        return (status >> 8);
    if (sig == SIGINT || sig == SIGQUIT)
        write(1, "\n", 1);
    return (128 + sig);
}

static void cleanup_pipes(int **pipes, int cmd_count)
{
    int i;

    i = 0;
    if (!pipes)
        return;
    while (i < cmd_count - 1)
    {
        if (pipes[i])
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
            free(pipes[i]);
        }
        i++;
    }
    free(pipes);
}

void free_pipe_data(t_pipe_data *data)
{
    if (!data)
        return;
    cleanup_pipes(data->pipes, data->cmd_count);
    data->pipes = NULL;
    free(data);
}

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

static void free_env_list(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

void    free_tokens(t_token *token)
{
    t_token *next;

    while (token)
    {
        next = token->next;
        if (token->value)
            free(token->value);
        free(token);
        token = next;
    }
}

void cleanup_simple(t_shell *shell)
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

void cleanup_shell(t_shell *shell)
{
    cleanup_simple(shell);
	if (shell->env_list)
    {
		free_env_list(shell->env_list);
        shell->env_list = NULL;
    }
    free(shell);
}
