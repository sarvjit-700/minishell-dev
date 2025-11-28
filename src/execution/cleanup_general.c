/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_general.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 08:21:19 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/28 08:22:31 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	extract_exit_code(int status)
{
	int	sig;
	int	core;

	sig = status & 0x7F;
	core = status & 0x80;
	if (sig == 0)
		return (status >> 8);
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		return (128 + sig);
	}
	if (sig == SIGQUIT)
	{
		if (core)
			write(2, "Quit (core dumped)\n", 20);
		else
			write(1, "\n", 1);
		return (128 + sig);
	}
	return (128 + sig);
}

static void	cleanup_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	if (!pipes)
		return ;
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

void	free_pipe_data(t_pipe_data *data)
{
	if (!data)
		return ;
	cleanup_pipes(data->pipes, data->cmd_count);
	data->pipes = NULL;
	free(data);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_env_array(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
