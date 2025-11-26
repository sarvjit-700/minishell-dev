/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 23:35:17 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/25 10:21:59 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				g_exit_code = 0;

int	execute(t_shell *shell, char **envp)
{
	t_cmd	*cmd_list;

	cmd_list = shell->cmd_list;
	if (!cmd_list)
		return (0);
	if (process_heredocs(cmd_list) == -1)
		return (g_exit_code);
	if (cmd_list->next)
		return (init_pipe_data(shell->cmd_list, envp, &shell->env_list));
	else
		return (execute_command(shell, cmd_list, envp));
}

static t_env	*create_env_node(const char *entry)
{
	t_env	*node;
	char	*eq;

	node = malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	eq = ft_strchr(entry, '=');
	if (eq)
	{
		node->key = ft_substr(entry, 0, eq - entry);
		node->value = ft_strdup(eq + 1);
	}
	else
	{
		node->key = ft_strdup(entry);
		node->value = NULL;
	}
	node->exported = true;
	node->next = NULL;
	return (node);
}

static t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*node;
	int		i;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		node = create_env_node(envp[i]);
		if (!node)
			return (NULL);
		node->next = head;
		head = node;
		i++;
	}
	return (head);
}

static int	init_shell(t_shell *shell, char **envp)
{
	shell->env_list = env_init(envp);
	if (!shell->env_list)
	{
		perror("minishell");
		return (1);
	}
	adjust_shlvl(&shell->env_list);
	shell->tokens = NULL;
	shell->cmd_list = NULL;
	shell->line = NULL;
	shell->last_status = 0;
	shell->running = 1;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)argv;
	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("minishell");
		return (1);
	}
	memset(shell, 0, sizeof(t_shell));
	if (init_shell(shell, envp))
		return (1);
	run_shell(shell, envp);
	cleanup_shell(shell);
	exit(g_exit_code);
}
