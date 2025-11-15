/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:44:47 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/14 18:18:37 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_cd_error(const char *target, int err_code)
{
	if (err_code == 1)
	{
		write(2, "minishell: cd: HOME not set", 27);
	}
	else if (err_code == 2)
	{
		write(2, "minishell: cd: OLDPWD not set", 29);
	}
	else if (err_code == 3)
	{
		write(2, "minishell: cd: ", 15);
		write(2, target, ft_strlen(target));
		write(2, ": No such file or directory\n", 28);
	}
	else if (err_code == 4)
	{
		write(2, "minishell: cd: ", 15);
		write(2, target, ft_strlen(target));
		write(2, ": Not a directory\n", 18);
	}
	else if (err_code == 5)
	{
		write(2, "minishell: cd: ", 15);
		write(2, target, ft_strlen(target));
		write(2, ": Permission denied\n", 20);
	}
	else
		perror("minishell: cd");
	return (1);
}

int	builtin_cd(t_cmd *cmd, t_env **env_list)
{
	char *target;
	char *oldpwd;
	char cwd[1024];
	struct stat sb;

	oldpwd = get_env_value(*env_list, "PWD");
	if (!cmd->argv[1] || ft_strcmp(cmd->argv[1], "~") == 0)
	{
		target = get_env_value(*env_list, "HOME");
		if (!target)
			return (print_cd_error(NULL, 1));
	}
	else if (cmd->argv[1] && ft_strcmp(cmd->argv[1], "-") == 0)
	{
		target = get_env_value(*env_list, "OLDPWD");
		if (!target)
			return (print_cd_error(NULL, 2));
	}
	else
		target = cmd->argv[1];
	if (stat(target, &sb) == -1)
		return (print_cd_error(target, 3));
	if (!S_ISDIR(sb.st_mode))
		return (print_cd_error(target, 4));
	if (access(target, X_OK) == -1)
		return (print_cd_error(target, 5));
	if (chdir(target) == -1)
		return (print_cd_error(target, 5));
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (print_cd_error(NULL, 0));
	set_env(env_list, "OLDPWD", oldpwd); // Store previous PWD into OLDPWD
	set_env(env_list, "PWD", cwd);       // Update PWD to new directory
	if (cmd->argv[1] && ft_strcmp(cmd->argv[1], "-") == 0)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
	}
	return (0);
}