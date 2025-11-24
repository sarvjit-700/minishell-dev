/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:05:47 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/19 18:05:47 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (expand_var(dst, buf));
}

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
		free(tmp);
		free(var);
		return (res);
	}
	res = expand_char(res, '$');
	(*i)++;
	return (res);
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
		if (str[i] == '$')
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

/* -------------------------------------------------------------------------- */
/*                          BUILD NEW ARGV HELPERS                            */
/* -------------------------------------------------------------------------- */

static char	**alloc_new_argv(int size)
{
	char	**arr;

	arr = malloc(sizeof(char *) * (size + 1));
	if (!arr)
		return (NULL);
	return (arr);
}

static void	free_words(char **words)
{
	int	i;

	i = 0;
	while (words && words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
}

static int	copy_words(char **dest, char **words, int idx)
{
	int	i;

	i = 0;
	while (words && words[i])
	{
		if (words[i][0] != '\0')
		{
			dest[idx] = ft_strdup(words[i]);
			if (!dest[idx])
				return (-1);
			idx++;
		}
		i++;
	}
	return (idx);
}

static int	expand_one_arg(char **new_argv, int idx, char *arg, t_shell *sh)
{
	char	*expanded;
	char	**words;

	expanded = expand_string(arg, sh);
	if (!expanded)
		return (-1);
	words = ft_split(expanded, ' ');
	free(expanded);
	if (!words)
		return (-1);
	idx = copy_words(new_argv, words, idx);
	free_words(words);
	return (idx);
}

/* -------------------------------------------------------------------------- */
/*                           REPLACE OLD ARGV                                 */
/* -------------------------------------------------------------------------- */

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
	char	**new_argv;
	int		idx;
	int		i;

	new_argv = alloc_new_argv(cmd->argc * 3 + 1);
	if (!new_argv)
		return ;
	idx = 0;
	i = 0;
	while (cmd->argv[i])
	{
		idx = expand_one_arg(new_argv, idx, cmd->argv[i], shell);
		if (idx == -1)
		{
			free_argv(new_argv);
			return ;
		}
		i++;
	}
	new_argv[idx] = NULL;
	free_argv(cmd->argv);
	cmd->argv = new_argv;
	cmd->argc = idx;
}

/* -------------------------------------------------------------------------- */
/*                     EXPAND REDIRECTION FILENAMES                           */
/* -------------------------------------------------------------------------- */

static void	expand_redirs(t_cmd *cmd, t_shell *sh)
{
	t_redir	*r;
	char	*tmp;

	r = cmd->redir;
	while (r)
	{
		if (ft_strchr(r->filename, '$'))
		{
			tmp = expand_string(r->filename, sh);
			if (tmp)
			{
				free(r->filename);
				r->filename = tmp;
			}
		}
		r = r->next;
	}
}

/* -------------------------------------------------------------------------- */
/*                              PUBLIC FUNCTION                               */
/* -------------------------------------------------------------------------- */

void	expand_vars(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmd_list;
    int i = 0;
    while(cmd && cmd->argv[i])
    {
        printf("argv[%d]: '%s'\n", i, cmd->argv[i]);
        i++;
    }
	while (cmd)
	{
		expand_cmd_argv(cmd, shell);
		expand_redirs(cmd, shell);
		cmd = cmd->next;
	}
}

/*
static int	count_expanded_argv(t_cmd *cmd, t_shell *shell)
{
	int		i;
	int		count;
	char	*expanded;
	char	**words;

	i = 0;
	count = 0;
	while (cmd->argv[i])
	{
		expanded = expand_string(cmd->argv[i], shell);
		if (!expanded)
			return (-1);
        printf("i: '%d', expanded: '%s'\n", i, expanded);
		words = ft_split(expanded, ' ');
		free(expanded);
		if (!words)
			return (-1);
		count = process_expanded_words(words, NULL, count, MODE_COUNT);
        printf("count: '%d'\n", count);
		free_words(words);
		i++;
	}
	return (count);
}
*/
/* ========================== FILLING PASS ================================= */
/*
static int	fill_expanded_argv(t_cmd *cmd, t_shell *shell, char **new_argv)
{
	int		i;
	int		idx;
	char	*expanded;
	char	**words;

	i = 0;
	idx = 0;
	while (cmd->argv[i])
	{
		expanded = expand_string(cmd->argv[i], shell);
		if (!expanded)
			return (-1);
		words = ft_split(expanded, ' ');
		free(expanded);
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
} */

/* ========================== MAIN CONVERSION =============================== */
