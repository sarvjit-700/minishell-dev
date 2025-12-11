/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 09:53:04 by ssukhija          #+#    #+#             */
/*   Updated: 2025/12/11 13:17:58 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler(int sig)
{
	(void)sig;
	g_exit_code = 130;
	if (isatty(STDIN_FILENO))
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	sigquit_handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
}

void	heredoc_sigint(int sig)
{
	(void)sig;
	g_exit_code = 130;
	write (1, "\n", 1);
	close(STDIN_FILENO);
}

void	setup_signal_handlers(int sig_type)
{
	if (sig_type == 1)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (sig_type == 2)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, sigquit_handler);
	}
}
