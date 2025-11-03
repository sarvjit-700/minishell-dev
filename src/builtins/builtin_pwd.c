/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:38:25 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/20 09:38:25 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_pwd(void) //move this to another file later
{
    char    cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("minishell: pwd");
        return (1);
    }
    printf("%s\n", cwd);
    return (0);
}