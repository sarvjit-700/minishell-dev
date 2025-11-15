/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 10:33:05 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/15 10:33:05 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*new_command(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (new_cmd == NULL)
		return (NULL);
	new_cmd->argv = NULL;
	new_cmd->argc = 0;
	new_cmd->redir = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

static int	add_arg(t_cmd *cmd, const char *arg)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (cmd->argc + 2));
	if (new_argv == NULL)
		return (0);
	i = 0;
	while (i < cmd->argc)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(arg);
	if (new_argv[i] == NULL)
	{
		free(new_argv);
		return (0);
	}
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	cmd->argc++;
	return (1);
}

int	start_cmd(t_parser_state *ps)
{
	if (ps->curr)
		return (1);
	ps->curr = new_command();
	if (ps->curr == NULL)
		return (0);
	if (ps->head == NULL)
	{
		ps->tail = ps->curr;
		ps->head = ps->tail;
	}
	else
	{
		ps->tail->next = ps->curr;
		ps->tail = ps->curr;
	}
	return (1);
}

int	handle_word(t_parser_state *ps)
{
	if (!start_cmd(ps))
		return (0);
	if (!add_arg(ps->curr, ps->token->value))
		return (0);
	return (1);
}