/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:30:34 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/24 10:30:34 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    adjust_shlvl(t_env **env_list)
{
    char    *old;
    long    level;
    char    *new_val;

    level = 1;
    old = get_env_value(*env_list, "SHLVL");
    if (old && *old != '\0')
    {
        level = ft_atol(old);
        level++;
    }
    if (level < 0)
        level = 0;
    else if (level > 1000)
        level = 1;
    new_val = ft_itoa(level);
    set_env(env_list, "SHLVL", new_val);
    free(new_val);
}
