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

int	exec_builtin(t_cmd *cmd, t_env *env_list)
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
	// if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
	// 	return (builtin_unset(cmd->argv, env_list));
	// if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
	// 	return (builtin_env(env_list));
	// if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
	// 	return (builtin_exit(cmd->argv));
	return (0); // Not a builtin
}