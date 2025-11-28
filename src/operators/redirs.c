/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 10:09:48 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/27 11:42:50 by ssukhija         ###   ########.fr       */
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

static int	handle_redir_type(t_redir *r)
{
	if (r->type == TOKEN_REDIR_IN)
		return (redir_in(r));
	else if (r->type == TOKEN_REDIR_OUT)
		return (redir_out_append(r, 1));
	else if (r->type == TOKEN_APPEND)
		return (redir_out_append(r, 2));
	else if (r->type == TOKEN_HEREDOC)
		return (redir_heredoc(r));
	printf("minishell: unknown redirection type\n");
	return (-1);
}

int	apply_redirs(t_cmd *cmd)
{
	t_redir	*r;
	int		ret;

	ret = 0;
	if (!cmd)
		return (0);
	r = cmd->redir;
	while (r)
	{
		ret = handle_redir_type(r);
		if (ret == -1)
			return (-1);
		r = r->next;
	}
	return (0);
}
