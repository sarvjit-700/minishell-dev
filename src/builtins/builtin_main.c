/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:46:41 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/25 21:30:36 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
	{
		return (0);
	}
	return (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

int	exec_builtin(t_cmd *cmd, t_shell *shell, t_env **env_list)
{
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		return (builtin_cd(cmd, env_list));
	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		return (builtin_export(cmd, env_list));
	if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		return (builtin_unset(cmd, env_list));
	if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		return (builtin_env(*env_list));
	if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		return (builtin_exit(cmd, shell));
	return (0);
}

char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

static void	set_new_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	memset(new, 0, sizeof(t_env));
	new->key = ft_strdup(key);
	if (!new->key)
	{
		free(new);
		return ;
	}
	if (value)
    {
        new->value = ft_strdup(value);
        if (!new->value)
        {
            free(new->key);
            free(new);
            return ;
        }
    }
    else
		new->value = NULL;
	new->exported = true;
	new->next = *env_list;
	*env_list = new;
}

/* above updated
static void	set_new_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	memset(new, 0, sizeof(t_env));
	new->key = ft_strdup(key);
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	if (value != NULL)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->exported = true;
	new->next = *env_list;
	*env_list = new;
} */

void	set_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*curr;

	curr = *env_list;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
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
