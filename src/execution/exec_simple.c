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


char    *get_env_value(t_env *env_list, const char *key)
{
    while (env_list)
    {
        if (ft_strncmp(env_list->key, key, ft_strlen(key)) == 0)
            return (env_list->value);
        env_list = env_list->next;
    }
    return (NULL);
}

int builtin_pwd(void) //move this to another file later
{
    char    cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("minishell: pwd");
        return (1);
    }
    printf("%s\n", cwd);
    return (0);
}

int execute_command(t_cmd *cmd, char **envp, t_env *env_list)
{
    char *path;
    pid_t pid;
    int status;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return (0);

    // Handle builtins directly
    if (is_builtin(cmd->argv[0]))
        return (exec_builtin(cmd, env_list));

    // External commands
    path = find_exec(cmd->argv[0], envp);
    if (!path)
        return (fprintf(stderr, "command not found: %s\n", cmd->argv[0])); //change fprintf

    pid = fork();
    if (pid == 0)
    {
        execve(path, cmd->argv, envp);
        perror("execve");
        exit(127);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        free(path);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
    }
    free(path);
    return (1);
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
	t_cmd	*cmds;

	(void)argc;
	(void)argv;
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
		cmds = parse_tokens(tokens);
        //expand_all_cmds(cmds, env_list); // need to implement later
		if (cmds)
			execute_command(cmds, envp, env_list);

		free_tokens(tokens);
		free_cmd_list(cmds);
		free(line);
	}
	free_env(env_list);
	return (0);
}
