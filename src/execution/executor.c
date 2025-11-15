/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 23:35:17 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/10 23:35:17 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
int	g_exit_code = 0;

//int execute_command(t_cmd *cmd, char **envp, t_env **env_list)
int execute_command(t_shell *shell, t_cmd *cmd, char **envp)
{
    char *path;
    pid_t pid;
    int status;
	int save_stdin;
	int	save_stdout;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return (0);

    // Handle builtins directly
    if (is_builtin(cmd->argv[0]) && !cmd->next)
	{
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
        g_exit_code = exec_builtin(cmd, &shell->env_list);
		dup2(save_stdin, STDIN_FILENO);
        dup2(save_stdout, STDOUT_FILENO);
        close(save_stdin);
        close(save_stdout);
        return (g_exit_code);
	}
    path = find_exec(cmd->argv[0], envp);

    if (!path && !is_builtin(cmd->argv[0]))
	{
		handle_exec_error(cmd->argv[0], 0);
		return (g_exit_code);
	}
    pid = fork();
	if (pid == -1)
	{
		perror("fork");
		g_exit_code = 1;
		return (g_exit_code);
	}
	if (pid == 0)
    {
		setup_signal_handlers(2);
		// signal(SIGINT, SIG_DFL);
        // signal(SIGQUIT, SIG_DFL);
		if (apply_redirs(cmd) == -1)
			exit(1);
		if (is_builtin(cmd->argv[0]))
		{
			free(path);
			exit(exec_builtin(cmd, &shell->env_list));
		}
		execve(path, cmd->argv, envp);
		handle_exec_error(path, 0);
		exit(127);
    }
	waitpid(pid, &status, 0);
	g_exit_code = extract_exit_code(status);
	free(path);
    return (1);
}

// int execute(t_cmd *cmd_list, char **envp, t_env **env_list)
// {
// 	if (!cmd_list)
// 		return (0);
//     if (process_heredocs(cmd_list) == -1)
//         return (g_exit_code);
//     if (cmd_list->next)
//         return (init_pipe_data(cmd_list, envp, env_list));
//     else
//         return (execute_command(cmd_list, envp, env_list));
// }

int execute(t_shell *shell, char **envp)
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

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}


t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*node;
	char	*eq;
	int		i;

	i = 0;
    head = NULL;
	while (envp[i])
	{
		node = malloc(sizeof(t_env));
		if (node == NULL)
			return (NULL);
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			node->key = ft_substr(envp[i], 0, eq - envp[i]);
			node->value = ft_strdup(eq + 1);
		}
		else
		{
			node->key = ft_strdup(envp[i]);
			node->value = NULL;
		}
		node->exported = true;
		node->next = head;
		head = node;
		i++;
	}
	return (head);
}

int init_shell(t_shell *shell, char **envp)
{
    shell->env_list = env_init(envp);
    if (!shell->env_list)
    {
        perror("minishell");
        return (1);
    }
	shell->tokens = NULL;
	shell->cmd_list = NULL;
	shell->line = NULL;
	shell->last_status = 0;
	shell->running = 1;
    return (0);
}

static t_status	parse_input(t_shell *shell)
{
	shell->cmd_list = parse_tokens(shell->tokens);
	if (!shell->cmd_list) 
	{
		free_tokens(shell->tokens);
		free(shell->line);
		return (LOOP_CONTINUE);
	}
	return (LOOP_OK);
}

static t_status	tokenize_input(t_shell *shell)
{	
	const char *input;  

	input = shell->line;              // ✅ make a const char* copy
	shell->tokens = tokenize(&input); // ✅ pass pointer to pointer
	if (!shell->tokens) 
	{
		free(shell->line);
		return (LOOP_CONTINUE);
	}
	return (LOOP_OK);
}

static t_status read_input(t_shell *shell)
{
	char *prompt = COLOUR "minishell$ " RESET;
	int len;

	setup_signal_handlers(0);
	if (isatty(STDIN_FILENO))
		shell->line = readline(prompt);
	else
	{
		shell->line = get_next_line(STDIN_FILENO);
		if (!shell->line)
			return (LOOP_BREAK);
		len = ft_strlen(shell->line);
		if (len > 0 && shell->line[len - 1] == '\n')
			shell->line[len - 1] = '\0';
	}
	if (!shell->line)
	{
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		return (LOOP_BREAK);
	}
	if (*shell->line)
		add_history(shell->line);
	else 
	{
		free(shell->line);
		return (LOOP_CONTINUE);
	}
	return (LOOP_OK);
}

void	run_shell(t_shell *shell, char **envp)
{
	t_status	status;
	t_redir 	*r;

	while (shell->running)
	{
		status = read_input(shell);
		if (status == LOOP_BREAK) break;
		if (status == LOOP_CONTINUE) continue;
		status = tokenize_input(shell);
		if (status == LOOP_CONTINUE) continue;
		status = parse_input(shell);
		if (status == LOOP_CONTINUE) continue;
			r = shell->cmd_list->redir;
		while (r)
			r = r->next;
		expand_vars(shell);
		setup_signal_handlers(1);
		//execute(shell->cmd_list, envp, &shell->env_list);
		execute(shell, envp);
		cleanup_simple(shell);
	}
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
	if (init_shell(shell, envp))
		return (1);
	run_shell(shell, envp);
	cleanup_shell(shell);
	return (0);
}
