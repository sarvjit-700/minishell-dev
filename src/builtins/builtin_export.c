/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:44:03 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/18 10:58:07 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_var(char *var)
{
	int	i;

	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
	{
		printf("NEED ERROR MSG\n");
		return (0);
	}
	i = 1;
	while (var[i] != '\0')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
		{
			printf("NEED SOME ERROR MSG\n");
			return (0);
		}
		i++;
	}
	return (1);
}

void	print_exported_vars(t_env *env_list)
{
	while (env_list)
	{
		printf("declare -x %s", env_list->key);
		if (env_list->value)
			printf("=\"%s\"", env_list->value);
		printf("\n");
		env_list = env_list->next;
	}
}

int	builtin_export(t_cmd *cmd, t_env **env_list)
{
	char	*eq;
	char	*key;
	char	*value;
	int		i;

	if (!cmd->argv[1])
	{
		print_exported_vars(*env_list);
		return (0);
	}
	i = 1;
	while (cmd->argv[i])
	{
		eq = ft_strchr(cmd->argv[i], '=');
		if (eq)
		{
			key = ft_substr(cmd->argv[i], 0, eq - cmd->argv[i]);
			value = ft_strdup(eq + 1);
			if (!is_valid_var(key))
				printf("minishell: export: '%s': not a valid identifier\n",
					cmd->argv[i]);
			else
				set_env(env_list, key, value);
			free(key);
			free(value);
		}
		else
		{
			if (!is_valid_var(cmd->argv[i]))
				printf("minishell: export: '%s': not a valid identifier\n",
					cmd->argv[i]);
			else
				set_env(env_list, cmd->argv[i], NULL);
		}
		i++;
	}
	return (0);
}