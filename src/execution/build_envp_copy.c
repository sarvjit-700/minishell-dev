/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_envp_copy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 22:18:10 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/26 22:18:10 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_env_entry(t_env *env)
{
	int		len;
	char	*entry;

	len = ft_strlen(env->key) + 2;
	if (env->value)
		len += ft_strlen(env->value);
	entry = malloc(len);
	if (!entry)
		return (NULL);
	entry[0] = '\0';
	ft_strlcat(entry, env->key, len);
	ft_strlcat(entry, "=", len);
	if (env->value)
		ft_strlcat(entry, env->value, len);
	return (entry);
}

static int	count_exported(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported)
			count++;
		env = env->next;
	}
	return (count);
}

static int	fill_envp(t_env *env, char **envp)
{
	int	i;

	i = 0;
	while (env)
	{
		if (env->exported)
		{
			envp[i] = build_env_entry(env);
			if (!envp[i])
				return (-1);
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (0);
}

char	**env_to_array(t_env *env_list)
{
	int		count;
	char	**envp;

	count = count_exported(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	if (fill_envp(env_list, envp) == -1)
	{
		free_env_array(envp);
		return (NULL);
	}
	return (envp);
}
