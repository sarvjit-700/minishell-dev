/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 20:50:52 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/11 20:50:52 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_exit(int type, int child)
{
	if (type == 126)
	{
		if (child)
			_exit(126);
		else
			g_exit_code = 126;
	}
	else
	{
		if (child)
			_exit(127);
		else
			g_exit_code = 127;
	}
}

static void	no_file_err(const char *path, int child)
{
	if (ft_strchr(path, ':') || ft_strchr(path, '/'))
	{
		write(2, "minishell: ", 11);
		write(2, path, ft_strlen(path));
		write(2, ": No such file or directory\n", 28);
	}
	else
	{
		write(2, path, ft_strlen(path));
		write(2, ": command not found\n", 20);
	}
	set_exit(127, child);
}

void	handle_exec_error(const char *path, int child)
{
	struct stat	sb;

	if (stat(path, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			write(2, "minishell: ", 11);
			write(2, path, ft_strlen(path));
			write(2, ": Is a directory\n", 17);
			set_exit(126, child);
		}
		else
		{
			write(2, "minishell: ", 11);
			write(2, path, ft_strlen(path));
			write(2, ": Permission denied\n", 20);
			set_exit(126, child);
		}
	}
	else
	{
		no_file_err(path, child);
	}
}
