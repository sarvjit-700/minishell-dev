/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:26:05 by ssukhija          #+#    #+#             */
/*   Updated: 2025/12/11 14:26:05 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_pipe_exit(t_pipe_data *data, char **envp, int code)
{
	cleanup_shell(data->shell);
	free_pipe_data(data);
	free_env_array(envp);
	exit(code);
}

void	clean_child_exit(t_shell *shell, char *path, int code)
{
	cleanup_shell(shell);
	free(path);
	exit(code);
}

void	hd_exit(t_shell *shell, int write_fd, int code)
{
	rl_clear_history();
	close(write_fd);
	cleanup_shell(shell);
	exit(code);
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
