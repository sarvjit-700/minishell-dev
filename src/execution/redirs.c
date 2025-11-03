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

#include "../includes/minishell.h"

static int handle_heredoc_stub(const char *delimiter)
{
    (void)delimiter;
    errno = EINVAL;
//    printf("HEREDOC\n");
    return (-1);
}

static int redir_in(t_redir *r)
{
    int fd;

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

static int redir_out_append(t_redir *r, int x)
{
    int fd;

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


static int redir_heredoc(t_redir *r)
{
    int fd;

    fd = handle_heredoc_stub(r->filename);
    if (fd == -1)
        return (-1);
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

int apply_redirs(t_cmd *cmd)
{
    t_redir *r;
    int     ret;

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