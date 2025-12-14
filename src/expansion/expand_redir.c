/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 09:39:10 by ssukhija          #+#    #+#             */
/*   Updated: 2025/12/14 11:20:47 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_char(char *dst, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (expand_var(dst, buf));
}

static int	print_ambiguous(char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	return (-1);
}

static int	is_ambiguous(char *expanded)
{
	if (expanded[0] == '\0')
		return (1);
	if (ft_strchr(expanded, ' '))
		return (1);
	return (0);
}

static int	process_single_redir(t_redir *r, t_shell *shell)
{
	char	*expanded;

	if (!r || !r->filename)
		return (0);
	if (!ft_strchr(r->filename, '$'))
		return (0);
	expanded = expand_string(r->filename, shell);
	if (!expanded)
		return (-1);
	if (is_ambiguous(expanded))
	{
		print_ambiguous(r->filename);
		free(expanded);
		return (-1);
	}
	free(r->filename);
	r->filename = expanded;
	return (0);
}

int	expand_redirs(t_cmd *cmd, t_shell *shell)
{
	t_redir	*r;
	int		ret;

	if (!cmd)
		return (0);
	r = cmd->redir;
	if (!r)
		return (0);
	while (r)
	{
		ret = process_single_redir(r, shell);
		if (ret == -1)
		{
			g_exit_code = 1;
			cmd->redir_error = 1;
			return (-1);
		}
		r = r->next;
	}
	return (0);
}
