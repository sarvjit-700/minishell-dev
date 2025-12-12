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

static char	**expand_and_split(const char *input, t_shell *shell)
{
	char	*expanded;
	char	**words;

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
	words = ft_split(expanded, ' ');
	free(expanded);
	return (words);
}

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
		count = process_expanded(words, NULL, count, MODE_COUNT);
		free_words(words);
		i++;
	}
	return (count);
}

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
		idx = process_expanded(words, new_argv, idx, MODE_FILL);
		free_words(words);
		if (idx == -1)
			return (-1);
		i++;
	}
	new_argv[idx] = NULL;
	return (idx);
}

static void	expand_cmd_argv(t_cmd *cmd, t_shell *shell)
{
	int		final_count;
	char	**new_argv;

	final_count = count_expanded_argv(cmd, shell);
	if (final_count == 0)
	{
		free_argv(cmd->argv);
		cmd->argv = NULL;
		cmd->argc = 0;
		return ;
	}
	if (final_count < 0)
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

void	expand_variables(t_shell *shell)
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
