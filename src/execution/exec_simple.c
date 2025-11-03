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

int execute_command(t_cmd *cmd, char **envp, t_env **env_list)
{
    char *path;
    pid_t pid;
    int status;
	int save_stdin;
	int	save_stdout;
	int ret;

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
        ret = exec_builtin(cmd, env_list);
		dup2(save_stdin, STDIN_FILENO);
        dup2(save_stdout, STDOUT_FILENO);
        close(save_stdin);
        close(save_stdout);
        return (ret);
	}
    // External commands
    path = find_exec(cmd->argv[0], envp);

    if (!path && !is_builtin(cmd->argv[0]))
	{
		printf("minishell: command not found: %s\n", cmd->argv[0]); //change fprintf
        return (127); 
	}
    pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
    {
		if (apply_redirs(cmd) == -1)
			exit(-1);
		if (is_builtin(cmd->argv[0]))
			exit(exec_builtin(cmd, env_list));
		execve(path, cmd->argv, envp);
	// perror(cmd->argv[0]);
		perror("execve");
		exit(127);
    }
	// else if (pid > 0)
    // {
	// 	waitpid(pid, &status, 0);
	// 	free(path);
	// 	if (WIFEXITED(status))
	// 		return (WEXITSTATUS(status));
    //     // perror("fork");
    //     // free(path);
    //     // return (1);
    // }

	free(path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	// if (WIFSIGNALED(status))
	// 	return (128 + WTERMSIG(status));
    return (1);
}

int execute(t_cmd *cmd_list, char **envp, t_env **env_list)
{
	if (!cmd_list)
		return (0);
	if (cmd_list->next)
	{
		//printf("here in pipeline\n");
		return (init_pipe_data(cmd_list, envp, env_list));
	}
	else
	{
		return (execute_command(cmd_list, envp, env_list));
	}
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

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	char	*line;
	const char *input;   // note: const because tokenize takes const char **
	t_token	*tokens;
	t_cmd	*cmd_list;

	(void)argc;
	(void)argv;
	env_list = NULL;
	env_list = env_init(envp);
	if (!env_list)
	{
		printf("minishell: failed to initialize environment\n");
		return (1);
	}
	while (1)
	{
		line = readline("minishell$ ");
		if (!line) // Ctrl+D
		{
			ft_putstr_fd("exit\n", 1);
			break;
		}

		if (*line) // only add non-empty lines
			add_history(line);

		input = line;              // ✅ make a const char* copy
		tokens = tokenize(&input); // ✅ pass pointer to pointer
		if (!tokens) 
		{
			free(line);
			continue;
		}
		cmd_list = parse_tokens(tokens);
		if (!cmd_list) 
		{
			free_tokens(tokens);
			free(line);
			continue;
		}
		t_redir *r = cmd_list->redir;
		while (r)
		{
			r = r->next;
		}
        //expand_all_cmds(cmds, env_list); // need to implement later
		// if (cmds)
		// 	execute_command(cmds, envp, &env_list);
		if (cmd_list)
		{
			//printf("Executing: %s\n", cmd_list->argv[0]);
			execute(cmd_list, envp, &env_list);
		}

		free_tokens(tokens);
		free_cmd_list(cmd_list);
		free(line);
	}
	free_env(env_list);
	return (0);
}
