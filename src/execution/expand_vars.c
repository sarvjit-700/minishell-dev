/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 12:04:29 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/10 12:04:29 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Split an expanded argv into multiple arguments if it contains spaces */

void cmd_append_argv(t_cmd *cmd, char **words, int start)
{
    int i, old_len, new_len;
    char **new_argv;

    old_len = 0;
    while (cmd->argv && cmd->argv[old_len])
        old_len++;

    new_len = old_len;
    for (i = start; words[i]; i++)
        new_len++;

    new_argv = malloc(sizeof(char *) * (new_len + 1));
    if (!new_argv)
        return;

    // copy old argv
    for (i = 0; i < old_len; i++)
        new_argv[i] = cmd->argv[i];

    // copy new words
    int j = old_len;
    for (i = start; words[i]; i++)
        new_argv[j++] = words[i];

    new_argv[j] = NULL;

    free(cmd->argv);
    cmd->argv = new_argv;
}


static char *expand_var(char *s1, char *s2)
{
    char *res;

    if (!s1 || !s2)
        return (NULL);
    res = ft_strjoin(s1, s2);
    free(s1);
    return (res);
}

static char *expand_char(char *s, char c)
{
    char *tmp;
    char str[2];

    str[0] = c;
    str[1] = '\0';
    tmp = expand_var(s, str);
    return (tmp);
}

static char *get_exitcode_str()
{
    //printf("getting exit code\n");
    return (ft_itoa(g_exit_code));
}

static char *dup_env_value(t_env *env_list, const char *key)
{
    char *val;

    val = get_env_value(env_list, key);
    if (!val)
        return (ft_strdup(""));
    return (ft_strdup(val));
}

static char *expand_string(char *str, t_shell *shell)
{
    //(void)shell;
    int     i;
    int     start;
    char    *res;
    char    *tmp;
    char    *var;
   // char    *old;

    i = 0;
    res = ft_strdup("");
    while (str[i])
    {
        if (str[i] == '$')
        {
            if (str[i + 1] == '?')
            {
                tmp = get_exitcode_str();
                res = expand_var(res, tmp);
                free(tmp);
                i += 2;
            }
            else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
            {
                i++;
                start = i;
                while (ft_isalnum(str[i]) || str[i] == '_')
                    i++;
                var = ft_substr(str, start, i - start);
                tmp = dup_env_value(shell->env_list, var);
                res = expand_var(res, tmp);
                free(tmp);
                free(var);
            }
            else
            {
                // literal $
                res = expand_char(res, str[i++]);
            }
        }
        else
            res = expand_char(res, str[i++]);
    }
    return (res);
}

void    expand_vars(t_shell *shell)
{
    t_cmd   *cmd;
    t_redir *redir;
    char    *tmp;
    int     i;

    cmd = shell->cmd_list;
    while (cmd)
    {
        i = 0;
        while (cmd->argv && cmd->argv[i])
        {
        if (ft_strchr(cmd->argv[i], '$'))
        {
            tmp = expand_string(cmd->argv[i], shell); // expand $VAR
            free(cmd->argv[i]);

            // Split on spaces
            char **words = ft_split(tmp, ' ');
            free(tmp);

            if (words)
            {
                cmd->argv[i] = words[0];            // replace original with first word
                cmd_append_argv(cmd, words, 1);     // append remaining words to argv
                free(words);                         // free array container (not the strings)
            }
        }

            i++;
        }
        redir = cmd->redir;
        while (redir)
        {
            if (ft_strchr(redir->filename, '$'))
            {
                tmp = expand_string(redir->filename, shell);
                free(redir->filename);
                redir->filename = tmp;
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
}