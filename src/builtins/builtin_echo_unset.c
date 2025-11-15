/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:56:19 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/14 21:06:30 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i] != '\0')
		return (0);
	return (1);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (argv[i] && is_n_flag(argv[i]))
	{
		n_flag = 1;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (n_flag != 1)
		printf("\n");
	return (0);
}

static void	unset_env(t_env **env_list, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_list;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_list = curr->next;
			free(curr->key);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	builtin_unset(t_cmd *cmd, t_env **env_list)
{
	int	i;

	if (!cmd->argv[1])
		return (0);
	i = 1;
	while (cmd->argv[i])
	{
		unset_env(env_list, cmd->argv[i]);
		i++;
	}
	return (0);
}
