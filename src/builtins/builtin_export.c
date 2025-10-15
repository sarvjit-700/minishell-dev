/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:44:03 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/15 11:44:03 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int  is_valid_var(char *var)
{
    int i;

    if (!var || !ft_isalpha(var[0]) && var[0] != '_')
    {
        printf("error :not starting with alpha or under\n");
        return (0);
    }
    i = 1;
    while (var[i] != '\0')
    {
        if (!ft_isalnum(var[i]) && var[i] != '_')
        {
            printf("contains nonsense\n");
            return (0);
        }
        i++;
    }
    printf("done export\n");
    return (1);
}

int builtin_export(t_cmd *cmd, t_env *env_list)
{
    (void)env_list;
    is_valid_var(cmd->argv[1])
}