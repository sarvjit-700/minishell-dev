/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:47:32 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/27 11:47:32 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
static void run_heredoc_child(int write_fd, const char *delimiter)
{
    char    *line;
    size_t  len;

    setup_signal_handlers(2);
    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(1, "> ", 2);
        line = get_next_line(STDIN_FILENO);
        if (!line)
        {
            close(write_fd);
            exit(0);
        }
        len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            close(write_fd);
            exit(0);
        }
		if (len > 0) // Check if we null-terminated it
            line[len - 1] = '\n';
        write(write_fd, line, ft_strlen(line));
        free(line);
    }
}
*/

static void	run_heredoc_child(int write_fd, const char *delimiter, t_shell *shell)
{
	char	*line;

	setup_signal_handlers(2);
	while (1)
	{
		line = readline("> ");
		if (g_exit_code == 130)
        {
            if (line)
                free(line);
            rl_clear_history(); 
            close(write_fd);
			cleanup_shell(shell);
            exit(130);
        }
		if (!line)
		{
			close(write_fd);
			cleanup_shell(shell);
			exit(0);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			rl_clear_history();
			close(write_fd);
			cleanup_shell(shell);
			exit(0);
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}


static int	handle_heredoc_parent(pid_t pid, int *pipefd)
{
	int	status;
	int	read_fd;

	close(pipefd[1]);
	read_fd = pipefd[0];
	waitpid(pid, &status, 0);
	g_exit_code = extract_exit_code(status);
	if (g_exit_code == 130)
	{
		close(read_fd);
		return (-1);
	}
	return (read_fd);
}

static int	create_heredoc(const char *delimiter, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		g_exit_code = 1;
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		g_exit_code = 1;
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		run_heredoc_child(pipefd[1], delimiter, shell);
	return (handle_heredoc_parent(pid, pipefd));
}

int	process_heredocs(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*r;

	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->redir;
		while (r)
		{
			if (r->type == TOKEN_HEREDOC)
			{
				r->fd = create_heredoc(r->filename, shell);
				if (r->fd == -1)
				{
					if (g_exit_code != 130)
						g_exit_code = 1;
					return (-1);
				}
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
