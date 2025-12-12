/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 09:58:31 by ssukhija          #+#    #+#             */
/*   Updated: 2025/12/11 15:28:16 by ssukhija         ###   ########.fr       */
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

static void	exec_external(t_pipe_data *data, t_cmd *cmd, char **new_envp)
{
	char	*path;

	path = find_exec(cmd->argv[0], *data->env_list);
	if (!path && !is_builtin(cmd->argv[0]))
	{
		handle_exec_error(cmd->argv[0], 0);
		clean_pipe_exit(data, new_envp, g_exit_code);
	}
	execve(path, cmd->argv, new_envp);
	handle_exec_error(path, 0);
	free(path);
	clean_pipe_exit(data, new_envp, g_exit_code);
}

static void	exec_child(t_pipe_data *data, t_cmd *cmd, char **envp)
{
	int		j;
	int		exit_code;
	char	**new_envp;

	(void)envp;
	new_envp = env_to_array(*data->env_list);
	j = 0;
	while (j < data->cmd_count - 1)
	{
		close(data->pipes[j][0]);
		close(data->pipes[j][1]);
		j++;
	}
	if (apply_redirs(cmd) == -1)
		clean_pipe_exit(data, new_envp, 1);
	if (is_builtin(cmd->argv[0]))
	{
		exit_code = exec_builtin(cmd, NULL, data->env_list);
		clean_pipe_exit(data, new_envp, exit_code);
	}
	exec_external(data, cmd, new_envp);
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
