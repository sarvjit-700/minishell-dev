/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 10:09:48 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/20 10:09:48 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redir_in(t_redir *r)
{
	int	fd;

	fd = open(r->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(r->filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	redir_out_append(t_redir *r, int x)
{
	int	fd;

	if (x == 1)
		fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(r->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	redir_heredoc(t_redir *r)
{
	if (r->fd == -1)
		return (-1);
	if (dup2(r->fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(r->fd);
		return (-1);
	}
	close(r->fd);
	r->fd = -1;
	return (0);
}

int	apply_redirs(t_cmd *cmd)
{
	t_redir	*r;
	int		ret;

	ret = 0;
	if (!cmd)
		return (0);
	ret = 0;
	r = cmd->redir;
	while (r)
	{
		if (r->type == TOKEN_REDIR_IN)
			ret = redir_in(r);
		else if (r->type == TOKEN_REDIR_OUT)
			ret = redir_out_append(r, 1);
		else if (r->type == TOKEN_APPEND)
			ret = redir_out_append(r, 2);
		else if (r->type == TOKEN_HEREDOC)
			ret = redir_heredoc(r);
		else
		{
			printf("minishell: unknown redirection type\n");
			return (-1);
		}
		if (ret == -1)
			return (-1);
		r = r->next;
	}
	return (0);
}

int	create_heredoc(const char *delimiter)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
		char *line;

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
	{
		setup_signal_handlers(2);
		close(pipefd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line)
				exit(0);
			if (ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				exit(0);
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
	}
	// ---- parent ----
	close(pipefd[1]); // parent only reads
	waitpid(pid, &status, 0);
	g_exit_code = extract_exit_code(status);
	if (g_exit_code == 130)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]); // read-end of heredoc data
}

int	process_heredocs(t_cmd *cmd_list)
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
				r->fd = create_heredoc(r->filename); // filename = delimiter
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
