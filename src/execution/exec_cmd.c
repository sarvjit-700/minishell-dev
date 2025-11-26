/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:40:19 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/21 17:40:19 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_builtin(t_shell *shell, t_cmd *cmd)
{
	int	save_stdin;
	int	save_stdout;
	int	exit_code;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	if (apply_redirs(cmd) == -1)
	{
		dup2(save_stdin, STDIN_FILENO);
		dup2(save_stdout, STDOUT_FILENO);
		close(save_stdin);
		close(save_stdout);
		return (1);
	}
	exit_code = exec_builtin(cmd, shell, &shell->env_list);//was &shell->env_list
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdin);
	close(save_stdout);
	return (exit_code);
}

static void	run_exec_child(t_shell *shell, t_cmd *cmd, char *path, char **envp)
{
	setup_signal_handlers(2);
	if (apply_redirs(cmd) == -1)
		exit(1);
	if (is_builtin(cmd->argv[0]))
	{
		free(path);
		exit(exec_builtin(cmd, shell, &shell->env_list));
	}
	execve(path, cmd->argv, envp);
	handle_exec_error(cmd->argv[0], 0);
	free(path);
	exit(127);
}

static int	spawn_child(t_shell *shell, t_cmd *cmd, char *path, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		run_exec_child(shell, cmd, path, envp);
	}
	waitpid(pid, &status, 0);
	return (extract_exit_code(status));
}

int	execute_command(t_shell *shell, t_cmd *cmd, char **envp)
{
	char	*path;
	int		status;

	if (cmd->redir_error)
		return (g_exit_code);
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (is_builtin(cmd->argv[0]) && !cmd->next)
	{
		g_exit_code = run_builtin(shell, cmd);
		return (g_exit_code);
	}
	if (ft_strcmp(cmd->argv[0], "./minishell") == 0)
    {
        status = add_new_shell(shell->env_list);
		shell->last_status = status; 
        g_exit_code = status;
        return (status);
    }
	path = find_exec(cmd->argv[0], envp);
	if (!path)
	{
		handle_exec_error(cmd->argv[0], 0);
		return (g_exit_code);
	}
	g_exit_code = spawn_child(shell, cmd, path, envp);
	free(path);
	return (g_exit_code);
}
