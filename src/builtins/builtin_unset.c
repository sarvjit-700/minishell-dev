/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 12:44:48 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/18 12:44:48 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void unset_env(t_env **env_list, const char *key)
{
    t_env   *curr;
    t_env   *prev;

    curr = *env_list;
    prev = NULL;
    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
        {
            if (prev)
                prev->next = curr->next;
            else
                *env_list = curr->next; // means remove head
            free(curr->key);
            if (curr->value)
                free(curr->value);
            free(curr);
            return ;
        }
        prev = curr;
        curr = curr->next;
    }  
}

int builtin_unset(t_cmd *cmd, t_env **env_list)
{
    int i;

    if (!cmd->argv[1])
        return (0);
    i = 1; 
    while (cmd->argv[i])
    {
        unset_env(env_list, cmd->argv[i]);
        i++;
    }
    return (0);
}