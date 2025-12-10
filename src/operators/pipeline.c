/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:54:04 by ssukhija          #+#    #+#             */
/*   Updated: 2025/12/10 21:38:03 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	int		count;

	count = 0;
	cmd = cmd_list;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static int	close_wait(t_pipe_data *data, pid_t *pids)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < data->cmd_count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid");
		i++;
	}
	return (status);
}

static int	*create_pids(t_cmd *cmd_list, t_pipe_data *data, char **envp)
{
	t_cmd	*cmd;
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!pids)
		return (handle_ptr_err("malloc", 1));
	cmd = cmd_list;
	while (data->i < data->cmd_count)
	{
		pids[data->i] = fork();
		if (pids[data->i] == -1)
			return (handle_ptr_err("fork", 1));
		if (pids[data->i] == 0)
		{
			free(pids);
			child_process(data, cmd, envp);
		}
		cmd = cmd->next;
		data->i++;
	}
	return (pids);
}

static int	execute_pipeline(t_pipe_data *data, t_cmd *cmd_list, char **envp)
{
	pid_t	*pids;
	int		status;

	data->pipes = create_pipes(data);
	if (!data->pipes && data->cmd_count > 1)
		return (1);
	pids = create_pids(cmd_list, data, envp);
	if (!pids)
		return (1);
	close_parent_pipes(data);
	status = close_wait(data, pids);
	free(pids);
	return (status);
}

int	init_pipe_data(t_cmd *cmd_list, char **envp, t_env **env_list, t_shell *shell)
{
	t_pipe_data	*data;
	int			status;

	data = malloc(sizeof(t_pipe_data));
	if (!data)
		return (1);
	data->shell = shell;
	data->cmd_count = count_cmds(cmd_list);
	if (data->cmd_count == 0)
	{
		free(data);
		return (1);
	}
	data->i = 0;
	data->pipes = NULL;
	data->env_list = env_list;
	status = execute_pipeline(data, cmd_list, envp);
	free_pipe_data(data);
	g_exit_code = extract_exit_code(status);
	return (g_exit_code);
}
