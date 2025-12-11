/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:44:47 by ssukhija          #+#    #+#             */
/*   Updated: 2025/12/11 20:27:05 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_cd_error(const char *target, int err_code)
{
	if (err_code == 1)
		write(2, "minishell: cd: HOME not set", 27);
	else if (err_code == 2)
		write(2, "minishell: cd: OLDPWD not set", 29);
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

static char	*resolve_target(t_cmd *cmd, t_env **env_list)
{
	char	*arg;

	arg = cmd->argv[1];
	if (!arg || ft_strcmp(arg, "~") == 0)
		return (get_env_value(*env_list, "HOME"));
	if (ft_strcmp(arg, "-") == 0)
		return (get_env_value(*env_list, "OLDPWD"));
	return (arg);
}

static int	validate_target(char *target)
{
	struct stat	sb;

	if (!target)
		return (print_cd_error(NULL, 1));
	if (stat(target, &sb) == -1)
		return (print_cd_error(target, 3));
	if (!S_ISDIR(sb.st_mode))
		return (print_cd_error(target, 4));
	if (access(target, X_OK) == -1)
		return (print_cd_error(target, 5));
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_env **env_list)
{
	char	*target;
	char	*oldpwd;
	char	cwd[1024];

	if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	oldpwd = get_env_value(*env_list, "PWD");
	target = resolve_target(cmd, env_list);
	if (validate_target(target) != 0)
		return (1);
	if (chdir(target) == -1)
		return (print_cd_error(target, 5));
	if (!getcwd(cwd, sizeof(cwd)))
		return (print_cd_error(NULL, 0));
	set_env(env_list, "OLDPWD", oldpwd);
	set_env(env_list, "PWD", cwd);
	if (cmd->argv[1] && ft_strcmp(cmd->argv[1], "-") == 0)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
	}
	return (0);
}
