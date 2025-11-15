/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_pwd_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:48:32 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/14 21:05:12 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_env(t_env *env_list)
{
	while (env_list)
	{
		if (env_list->exported && env_list->value)
			printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

static int	is_num(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_cmd *cmd)
{
	int	exit_code;

	if (isatty(STDIN_FILENO))
		printf("exit\n");
	exit_code = 0;
	if (!cmd->argv[1])
		exit(0);
	if (!is_num(cmd->argv[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n",
			cmd->argv[1]);
		exit(255);
	}
	if (cmd->argv[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	exit_code = (unsigned char)ft_atoi(cmd->argv[1]);
	exit(exit_code % 256);
}
