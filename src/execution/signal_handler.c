/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 09:53:04 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/10 09:53:04 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sigint_handler(int sig)
{
    (void)sig;

    g_exit_code = 130;
    if (isatty(STDIN_FILENO))
    {    
    // Write a newline, clear current input, and redisplay prompt
        write(1, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

void sigquit_handler(int sig)
{
    (void)sig;
    // Do nothing when at prompt
    write(1, "Quit (core dumped)\n", 19);
    rl_on_new_line();
    rl_redisplay();
}

void setup_signal_handlers(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
}
