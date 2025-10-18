/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:44:47 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/18 10:47:47 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_cd(t_cmd *cmd, t_env **env_list)
{
	char	*target;
	char	*oldpwd;
	char	cwd[1024];

	oldpwd = get_env_value(*env_list, "PWD");
	if (!cmd->argv[1] || ft_strcmp(cmd->argv[1], "~") == 0)
	{
		target = get_env_value(*env_list, "HOME");
		if (!target)
		{
			perror("minishell: cd: HOME not set");
			return (1);
		}
	}
	else if (cmd->argv[1] && ft_strcmp(cmd->argv[1], "-") == 0)
	{
		target = get_env_value(*env_list, "OLDPWD");
		if (!target)
		{
			perror("minishell: cd: OLDPWD not set");
			return (1);
		}
	}
	else
		target = cmd->argv[1];
	if (chdir(target) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd");
		return (1);
	}
	set_env(env_list, "OLDPWD", oldpwd);// Store previous PWD into OLDPWD
	set_env(env_list, "PWD", cwd);// Update PWD to new directory
	return (0);
}