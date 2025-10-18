/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 13:48:32 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/18 13:48:32 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_env(t_env *env_list)
{
    while (env_list)
    {
        if (env_list->exported && env_list->value)
            printf("%s=%s\n", env_list->key, env_list->value);
        env_list = env_list->next;
    }
    return (0);
}