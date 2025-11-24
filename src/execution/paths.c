/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:36:34 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/18 10:48:26 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_parts(char **arr, char *cmd_path, int x)
{
	int	i;

	if (x == 1 && cmd_path)
		free(cmd_path);
	i = 0;
	if (!arr)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*get_env_path(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

static char	*direct_path(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*tmp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_parts(paths, "none", 0);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_parts(paths, "none", 0);
	return (NULL);
}

char	*find_exec(char *cmd, char **envp)
{
	char	*env_path;
	char	**paths;
	char	*full_path;

	if (cmd == NULL || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (direct_path(cmd));
	full_path = NULL;
	env_path = get_env_path(envp);
	if (env_path == NULL)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (paths == NULL)
		return (NULL);
	full_path = search_in_paths(paths, cmd);
	return (full_path);
}
