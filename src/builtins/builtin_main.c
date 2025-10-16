/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:46:41 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/15 10:46:41 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_builtin(const char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strncmp(cmd, "echo", 5)
        || !ft_strncmp(cmd, "cd", 3)
        || !ft_strncmp(cmd, "pwd", 4)
        || !ft_strncmp(cmd, "export", 7)
        || !ft_strncmp(cmd, "unset", 6)
        || !ft_strncmp(cmd, "env", 4)
        || !ft_strncmp(cmd, "exit", 5));
}

int	exec_builtin(t_cmd *cmd, t_env **env_list)
{
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
	 	return (builtin_cd(cmd));
	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		return (builtin_export(cmd, env_list));
	// if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
	// 	return (builtin_unset(cmd->argv, env_list));
	// if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
	// 	return (builtin_env(env_list));
	// if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
	// 	return (builtin_exit(cmd->argv));
	return (0); // Not a builtin
}


static void	set_new_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	new->key = ft_strdup(key);
	if (value != NULL)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->exported = true;
	new->next = *env_list;
	*env_list = new;
}

void	set_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*curr;

	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(key)) == 0)
		{
			free(curr->value);
			if (value != NULL)
				curr->value = ft_strdup(value);
			else
				curr->value = NULL;
			curr->exported = true;
			return ;
		}
		curr = curr->next;
	}
	set_new_env(env_list, key, value);
}
