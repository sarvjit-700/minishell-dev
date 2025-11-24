/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 09:51:11 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/18 10:45:23 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_status	parse_input(t_shell *shell)
{
	shell->cmd_list = parse_tokens(shell->tokens);
	if (!shell->cmd_list)
	{
		free_tokens(shell->tokens);
		free(shell->line);
		return (LOOP_CONTINUE);
	}
	return (LOOP_OK);
}

static t_status	tokenize_input(t_shell *shell)
{
	const char	*input;

	input = shell->line;
	shell->tokens = tokenize(&input);
	if (!shell->tokens)
	{
		free(shell->line);
		return (LOOP_CONTINUE);
	}
	return (LOOP_OK);
}

static char	*read_non_intract(void)
{
	char	*line;
	int		len;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

static t_status	read_input(t_shell *shell)
{
	char	*prompt;

	prompt = COLOUR "minishell$ " RESET;
	setup_signal_handlers(0);
	if (isatty(STDIN_FILENO))
		shell->line = readline(prompt);
	else
		shell->line = read_non_intract();
	if (!shell->line)
	{
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		return (LOOP_BREAK);
	}
	if (*shell->line)
		add_history(shell->line);
	else
	{
		free(shell->line);
		return (LOOP_CONTINUE);
	}
	return (LOOP_OK);
}

void	run_shell(t_shell *shell, char **envp)
{
	t_status	status;
	t_redir		*r;

	while (shell->running)
	{
		status = read_input(shell);
		if (status == LOOP_BREAK)
			break ;
		if (status == LOOP_CONTINUE)
			continue ;
		status = tokenize_input(shell);
		if (status == LOOP_CONTINUE)
			continue ;
		status = parse_input(shell);
		if (status == LOOP_CONTINUE)
			continue ;
		r = shell->cmd_list->redir;
		while (r)
			r = r->next;
		expand_vars(shell);
		setup_signal_handlers(1);
		execute(shell, envp);
		cleanup_simple(shell);
	}
}
