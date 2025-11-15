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

int execute_command(t_cmd *cmd, char **envp, t_env **env_list)
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
        g_exit_code = exec_builtin(cmd, env_list);
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
		signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
		if (apply_redirs(cmd) == -1)
			exit(1);
		if (is_builtin(cmd->argv[0]))
		{
			free(path);
			exit(exec_builtin(cmd, env_list));
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

int execute(t_cmd *cmd_list, char **envp, t_env **env_list)
{
	if (!cmd_list)
		return (0);
    if (process_heredocs(cmd_list) == -1)
        return (g_exit_code);
    if (cmd_list->next)
        return (init_pipe_data(cmd_list, envp, env_list));
    else
        return (execute_command(cmd_list, envp, env_list));
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

#define COLOUR   "\001\033[33m\002"
#define RESET   "\001\033[0m\002"

int	main(int argc, char **argv, char **envp)
{
	char *prompt = COLOUR "minishell$ " RESET;
	t_shell	*shell;
	t_redir *r;
	int len;
	const char *input;   // note: const because tokenize takes const char **

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
	while (shell->running)
	{
		//if (isatty(STDIN_FILENO))
		setup_signal_handlers();

		if (isatty(STDIN_FILENO))
			//shell->line = readline("minishell$ ");
			shell->line = readline(prompt);
		else
		{
			shell->line = get_next_line(STDIN_FILENO);
			if (!shell->line)
				break;
			// Remove trailing newline for consistency with readline()
			len = ft_strlen(shell->line);
			if (len > 0 && shell->line[len - 1] == '\n')
				shell->line[len - 1] = '\0';
		}
		if (!shell->line) // Ctrl+D or EOF
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			break;
		}

		if (*shell->line) // only add non-empty lines
			add_history(shell->line);
		else 
			{
				free(shell->line);
				continue;
			}

		input = shell->line;              // ✅ make a const char* copy
		shell->tokens = tokenize(&input); // ✅ pass pointer to pointer
		if (!shell->tokens) 
		{
			free(shell->line);
			continue;
		}
		shell->cmd_list = parse_tokens(shell->tokens);
		if (!shell->cmd_list) 
		{
			free_tokens(shell->tokens);
			free(shell->line);
			continue;
		}
		r = shell->cmd_list->redir;
		while (r)
		{
			r = r->next;
		}
		expand_vars(shell);
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (shell->cmd_list)   //probably don't need this if just execute
			execute(shell->cmd_list, envp, &shell->env_list);
		cleanup_simple(shell);
	}
	cleanup_shell(shell);
	return (0);
}
