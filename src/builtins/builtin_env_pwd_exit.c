/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_pwd_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:48:32 by ssukhija          #+#    #+#             */
/*   Updated: 2025/12/14 10:48:23 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	get_exit_val(const char *str, long long *val)
{
	int					sign;
	unsigned long long	res;
	int					i;

	i = 0;
	res = 0;
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		if ((res > 922337203685477580ULL)
			|| (res == 922337203685477580ULL
				&& (str[i] - '0') > (7 + (sign == -1))))
			return (0);
		res = res * 10 + (str[i++] - '0');
	}
	*val = (long long)(res * sign);
	return (1);
}

static int	handle_numeric_err(t_shell *shell, char *arg)
{
	printf("minishell: exit: %s: numeric argument required\n", arg);
	g_exit_code = 2;
	shell->running = 0;
	return (g_exit_code);
}

int	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	long long	res;

	if (isatty(STDIN_FILENO))
		printf("exit\n");
	if (!shell)
		return (g_exit_code);
	if (!cmd->argv[1])
	{
		shell->running = 0;
		return (g_exit_code);
	}
	if (!get_exit_val(cmd->argv[1], &res))
		return (handle_numeric_err(shell, cmd->argv[1]));
	if (cmd->argv[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	g_exit_code = (unsigned char)res;
	shell->running = 0;
	return (g_exit_code);
}
