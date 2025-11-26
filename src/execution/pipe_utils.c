/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 09:58:31 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/25 09:42:53 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	**create_pipe_arr(t_pipe_data *data)
{
	int	**pipes;

	if (data->cmd_count < 2)
		return (NULL);
	pipes = malloc(sizeof(int *) * (data->cmd_count - 1));
	if (!pipes)
	{
		perror("malloc");
		return (NULL);
	}
	return (pipes);
}

int	**create_pipes(t_pipe_data *data)
{
	int	**pipes;
	int	i;

	pipes = create_pipe_arr(data);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < data->cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (handle_ptr_err("malloc", 1));
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	close_parent_pipes(t_pipe_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		close(data->pipes[i][0]);
		close(data->pipes[i][1]);
		i++;
	}
}

static void	exec_child(t_pipe_data *data, t_cmd *cmd, char **envp)
{
	int		j;
	char	*path;

	j = 0;
	while (j < data->cmd_count - 1)
	{
		close(data->pipes[j][0]);
		close(data->pipes[j][1]);
		j++;
	}
	if (apply_redirs(cmd) == -1)
		exit(1);
	if (is_builtin(cmd->argv[0]))
		exit(exec_builtin(cmd, NULL, data->env_list));
	path = find_exec(cmd->argv[0], envp);
	if (!path && !is_builtin(cmd->argv[0]))
	{
		handle_exec_error(cmd->argv[0], 1);
		exit(127);
	}
	execve(path, cmd->argv, envp);
	handle_exec_error(path, 1);
	free(path);
	exit(127);
}

void	child_process(t_pipe_data *data, t_cmd *cmd, char **envp)
{
	setup_signal_handlers(2);
	if (data->i > 0)
		dup2(data->pipes[data->i - 1][0], STDIN_FILENO);
	if (data->i < data->cmd_count - 1)
		dup2(data->pipes[data->i][1], STDOUT_FILENO);
	exec_child(data, cmd, envp);
}
