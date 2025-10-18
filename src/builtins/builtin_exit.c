/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 14:20:58 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/18 15:21:22 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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