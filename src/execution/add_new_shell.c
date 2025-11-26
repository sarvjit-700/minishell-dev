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

t_env  *copy_env_list(t_env *src)
{
    t_env *new_list;
    t_env *node;

    new_list = NULL;
    while (src)
    {
        node = malloc(sizeof(t_env));
        if (!node)
            return (NULL);
        node->key = ft_strdup(src->key);
        if (src->value)
            node->value = ft_strdup(src->value);
        else
            node->value = NULL;
        node->exported = src->exported;
        node->next = new_list;
        new_list = node;
        src = src->next;
    }
    return (new_list);
}

static char *build_env_entry(t_env *env)
{
    int     len;
    char    *entry;

    len = ft_strlen(env->key) + 2;
    if (env->value)
        len += ft_strlen(env->value);
    entry = malloc(len);
    if (!entry)
        return (NULL);
    entry[0] = '\0';
    ft_strlcat(entry, env->key, len);
    ft_strlcat(entry, "=", len);
    if (env->value)
        ft_strlcat(entry, env->value, len);
    return (entry);
}


char    **env_to_array(t_env *env_list)
{
    int     count;
    t_env   *tmp;
    char    **envp;

    count = 0;
    tmp = env_list;
    while (tmp)
    {
        if (tmp->exported)
            count++;
        tmp = tmp->next;
    }
    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return (NULL);
    count = 0;
    while (env_list)
    {
        if (env_list->exported)
        {
            envp[count] = build_env_entry(env_list);
            if (!envp[count])
            {
                free_env_array(envp);
                return (NULL);
            }
            count++;
        }
        env_list = env_list->next;
    }
    envp[count] = NULL;
    return (envp);
}


void    adjust_shlvl(t_env **env_list)
{
    char    *old;
    long    level;
    char    *new_val;

    level = 0;
    old = get_env_value(*env_list, "SHLVL");
    if (old && *old != '\0')
        level = ft_atol(old);
    level = level + 1;
    if (level < 0)
        level = 0;
    else if (level > 1000)
        level = 1;
    new_val = ft_itoa(level);
    set_env(env_list, "SHLVL", new_val);
    free(new_val);
}

static void  new_exec_child(char **envp)
{
    char *argv_exec[2];

    argv_exec[0] = "./minishell";
    argv_exec[1] = NULL;
    execve("./minishell", argv_exec, envp);
    perror("execve");
    exit(1);
}

int    add_new_shell(t_env *parent)
{
    pid_t   pid;
    int     status;
    t_env   *child_env;
    char    **envp;

    child_env = copy_env_list(parent);
    if (!child_env)
        return (1);
    envp = env_to_array(child_env);
    pid = fork();
    if (pid == 0)
        new_exec_child(envp);
    waitpid(pid, &status, 0);
    free_env_array(envp);
    free_env_list(child_env);
    return (extract_exit_code(status));
}
