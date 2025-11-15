/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:44:03 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/14 21:00:28 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_var(char *var)
{
	int	i;

	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (0);
	i = 1;
	while (var[i] != '\0')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
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

static int	handle_export_assignment(char *arg, t_env **env_list)
{
	char	*eq;
	char	*key;
	char	*value;
	int		ret;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (0);
	key = ft_substr(arg, 0, eq - arg);
	value = ft_strdup(eq + 1);
	if (!is_valid_var(key))
	{
		printf("minishell: export: `%s': not a valid identifier\n", arg);
		ret = 1;
	}
	else
	{
		set_env(env_list, key, value);
		ret = 0;
	}
	free(key);
	free(value);
	return (ret);
}

static int	process_export_var(char *arg, t_env **env_list)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
		return (handle_export_assignment(arg, env_list));
	if (!is_valid_var(arg))
	{
		printf("minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	set_env(env_list, arg, NULL);
	return (0);
}

int	builtin_export(t_cmd *cmd, t_env **env_list)
{
	int	i;
	int	ret;
	int	status;

	ret = 0;
	if (!cmd->argv[1])
	{
		print_exported_vars(*env_list);
		return (0);
	}
	i = 1;
	while (cmd->argv[i])
	{
		status = process_export_var(cmd->argv[i], env_list);
		if (status != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
