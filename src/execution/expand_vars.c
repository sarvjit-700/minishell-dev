/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:35:19 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/19 19:35:19 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ============================ BASIC UTILITIES ============================= */

static char	*dup_env_value(t_env *env, const char *key)
{
	char	*val;

	val = get_env_value(env, key);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

static char	*expand_var(char *dst, char *src)
{
	char	*tmp;

	if (!dst)
		dst = ft_strdup("");
	if (!src)
		src = ft_strdup("");
	tmp = ft_strjoin(dst, src);
	free(dst);
	return (tmp);
}

static char	*expand_char(char *dst, char c)
{
	char buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (expand_var(dst, buf));
}

/* ============================ STRING EXPANSION ============================ */

static int	is_valid_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static char	*expand_dollar(char *str, int *i, t_shell *shell, char *res)
{
	char	*tmp;
	char	*var;
	int		start;

	if (str[*i + 1] == '?')
	{
		tmp = ft_itoa(g_exit_code);
		res = expand_var(res, tmp);
		free(tmp);
		*i += 2;
		return (res);
	}
	if (is_valid_start(str[*i + 1]))
	{
		(*i)++;
		start = *i;
		while (ft_isalnum(str[*i]) || str[*i] == '_')
			(*i)++;
		var = ft_substr(str, start, *i - start);
		tmp = dup_env_value(shell->env_list, var);
		res = expand_var(res, tmp);
		free(var);
		free(tmp);
		return (res);
	}
	res = expand_char(res, '$');
	(*i)++;
	return (res);
}

static char	*remove_markers(const char *s)
{
	char	*clean;
	int		i;
	int		j;

	clean = malloc(ft_strlen(s) + 1);
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == S_QM_S || s[i] == S_QM_E
		|| s[i] == D_QM_S || s[i] == D_QM_E)
		{
			i++;
			continue;
		}
		clean[j++] = s[i++];
	}
	clean[j] = '\0';
	return (clean);
}

static char	*expand_string(char *str, t_shell *shell)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == S_QM_S)
		{
			i++;
			while (str[i] && str[i] != S_QM_E)
			{
				res = expand_char(res, str[i]);
				i++;
			}
			if (str[i] == S_QM_E)
				i++;
		}
		else if (str[i] == '\\')
		{
			i++;
			if (str[i])
				res = expand_char(res, str[i++]);
		}
		else if (str[i] == '$')
			res = expand_dollar(str, &i, shell, res);
		else
		{
			res = expand_char(res, str[i]);
			i++;
		}
		if (!res)
			return (NULL);
	}
	return (res);
}

/* ======================== REUSABLE WORD PROCESSOR ========================= */

static int	process_expanded_words(char **words, char **dst, int idx, t_mode mode)
{
	int	i;

	i = 0;
	while (words && words[i])
	{
		if (mode == MODE_FILL)
		{
			dst[idx] = ft_strdup(words[i]);
			if (!dst[idx])
				return (-1);
		}
		idx++;
		i++;
	}
	return (idx);
}


static void	free_words(char **w)
{
	int	i;

	i = 0;
	while (w && w[i])
	{
		free(w[i]);
		i++;
	}
	free(w);
}

/*
static char **expand_and_split(const char *input, t_shell *shell)
{
    char    *expanded;
    char    **words;

    if (!input)
        return (NULL);

    expanded = expand_string((char *)input, shell);
    if (!expanded)
		return (NULL);
    if (ft_strchr(expanded, S_QM_S) || ft_strchr(expanded, D_QM_S))
    {
        words = ft_calloc(2, sizeof(char *));
        if (!words)
        {
            free(expanded);
            return (NULL);
        }
        words[0] = remove_markers(expanded);
        free(expanded);
        return (words);
    }
	if (expanded[0] == '\0')
	{
		words = malloc(sizeof(char*) * 2);
		if (!words)
		{
			free(expanded);
			return NULL;
		}
		words[0] = ft_strdup("");
		words[1] = NULL;
		free(expanded);
		return words;
	}
    words = ft_split(expanded, ' ');
    free(expanded);
    return (words);
}
	*/
static char **expand_and_split(const char *input, t_shell *shell)
{
    char *expanded;
    char **words;

    if (!input)
        return NULL;

    expanded = expand_string((char *)input, shell);
    if (!expanded)
        return NULL;

    if (ft_strchr(expanded, S_QM_S) || ft_strchr(expanded, D_QM_S))
    {
        words = ft_calloc(2, sizeof(char *));
        if (!words)
        {
            free(expanded);
            return NULL;
        }
        words[0] = remove_markers(expanded);
        free(expanded);
        return words;
    }

    words = ft_split(expanded, ' ');
    free(expanded);
    return words;
}



/* =========================== COUNTING PASS ================================ */

static int	count_expanded_argv(t_cmd *cmd, t_shell *shell)
{
	int		i;
	int		count;
	char	**words;

	i = 0;
	count = 0;
	while (cmd->argv[i])
	{
        words = expand_and_split(cmd->argv[i], shell);
		if (!words)
			return (-1);
		count = process_expanded_words(words, NULL, count, MODE_COUNT);
		free_words(words);
		i++;
	}
	return (count);
}

/* ========================== FILLING PASS ================================= */

static int	fill_expanded_argv(t_cmd *cmd, t_shell *shell, char **new_argv)
{
	int		i;
	int		idx;
	char	**words;

	i = 0;
	idx = 0;
	while (cmd->argv[i])
	{
		words = expand_and_split(cmd->argv[i], shell);
		if (!words)
			return (-1);
		idx = process_expanded_words(words, new_argv, idx, MODE_FILL);
		free_words(words);
		if (idx == -1)
			return (-1);
		i++;
	}
	new_argv[idx] = NULL;
	return (idx);
}

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	expand_cmd_argv(t_cmd *cmd, t_shell *shell)
{
	int		final_count;
	char	**new_argv;

	final_count = count_expanded_argv(cmd, shell);
	if (final_count <= 0)
		return ;
	new_argv = malloc(sizeof(char *) * (final_count + 1));
	if (!new_argv)
		return ;
	if (fill_expanded_argv(cmd, shell, new_argv) == -1)
	{
		free_argv(new_argv);
		return ;
	}
	free_argv(cmd->argv);
	cmd->argv = new_argv;
	cmd->argc = final_count;
}

/* ============================ REDIRECTION ================================= */

static int	expand_redirs(t_cmd *cmd, t_shell *shell)
{
	t_redir	*r;
	char	*tmp;

	r = cmd->redir;
	while (r)
	{
		if (ft_strchr(r->filename, '$'))
		{
			tmp = expand_string(r->filename, shell);
			if (tmp)
			{
				if (tmp[0] == '\0' || ft_strchr(tmp, ' '))
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(r->filename, 2);
					ft_putstr_fd(": ambiguous redirect\n", 2);
					g_exit_code = 1;
					cmd->redir_error = 1;
					free(tmp);
					return (-1);
				}
				else
				{
					free(r->filename);
					r->filename = tmp;
				}
			}
		}
		r = r->next;
	}
	return (0);
}

/* ============================ PUBLIC ENTRY ================================ */

void	expand_vars(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmd_list;
	while (cmd)
	{
		expand_cmd_argv(cmd, shell);
		expand_redirs(cmd, shell);
		cmd = cmd->next;
	}
}
