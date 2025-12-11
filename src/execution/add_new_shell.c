/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_new_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:30:34 by ssukhija          #+#    #+#             */
/*   Updated: 2025/12/11 20:40:26 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*copy_env_list(t_env *src)
{
	t_env	*new_list;
	t_env	*node;

	new_list = NULL;
	while (src)
	{
		node = malloc(sizeof(t_env));
		if (!node)
			return (NULL);
		node->key = ft_strdup(src->key);
		if (src->value)
			node->value = ft_strdup(src->value);
		else
			node->value = NULL;
		node->exported = src->exported;
		node->next = new_list;
		new_list = node;
		src = src->next;
	}
	return (new_list);
}

void	adjust_shlvl(t_env **env_list)
{
	char	*old_val;
	long	level;
	char	*new_val;

	level = 0;
	old_val = get_env_value(*env_list, "SHLVL");
	if (old_val && *old_val != '\0')
		level = ft_atol(old_val);
	old_val = NULL;
	level = level + 1;
	if (level < 0)
		level = 0;
	else if (level > 1000)
		level = 1;
	new_val = ft_itoa(level);
	set_env(env_list, "SHLVL", new_val);
	free(new_val);
}

static void	new_exec_child(t_shell *shell, t_env *child_env, char **envp)
{
	char	*argv_exec[2];

	argv_exec[0] = "./minishell";
	argv_exec[1] = NULL;
	execve("./minishell", argv_exec, envp);
	perror("execve");
	free_env_array(envp);
	free_env_list(child_env);
	cleanup_shell(shell);
	exit(1);
}

int	add_new_shell(t_shell *shell)
{
	pid_t	pid;
	int		status;
	t_env	*child_env;
	char	**envp;

	child_env = copy_env_list(shell->env_list);
	if (!child_env)
		return (1);
	envp = env_to_array(child_env);
	pid = fork();
	if (pid == 0)
		new_exec_child(shell, child_env, envp);
	waitpid(pid, &status, 0);
	free_env_array(envp);
	free_env_list(child_env);
	return (extract_exit_code(status));
}
