/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:44:47 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/15 10:44:47 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_cd(t_cmd *cmd)
{
    const char  *home;
    const char  *oldpwd;
    char    cwd[1024];

    if (!cmd->argv[1] || ft_strncmp(cmd->argv[1], "~", 1) == 0)
    {
        home = getenv("HOME");
        if (!home)
        {
            perror("minishell: cd: HOME not set");
            return (1);
        } 
        if (chdir(home) == -1)
        {
            perror("minishell: cd");
            return (1);
        }
    }
    else if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-", 1) == 0)
    {
        oldpwd = getenv("OLDPWD");
        if (!oldpwd)
        {
            perror("minishell: cd: OLDPWD not set");
            return (1);
        }
        if (chdir(oldpwd) == -1)
        {
            perror("minishell: cd");
            return (1);
        }
        // need to display home DIR\n before prompt
        printf("DISPLAY PREV DIR HERE\n");
    }
    else
    {
        if (chdir(cmd->argv[1]) == -1)
        {
            perror("minishell: cd");
            return (1);
        }
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("minishell: cd");
        return (1);
    }

    setenv("OLDPWD", getenv("PWD"), 1);
    setenv("PWD", cwd, 1);
    return (0);
}
