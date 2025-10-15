/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:56:19 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/15 10:56:19 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-' || arg[1] != 'n') //shoudl be -n to cont.
		return (0);
	i = 2;
	while (arg[i] && arg[i] == 'n') // check for more n's
		i++;
	if (arg[i] != '\0') // if not at the end mean something is after n get out
		return (0);
	return (1); // 1 only if there is -n
}

int builtin_echo(char **argv)
{
    int i;
    int n_flag;

    i = 1;
    n_flag = 0;
    while (argv[i] && is_n_flag(argv[i])) // look for all -n
    {
        n_flag = 1;
        i++;
    }
    while (argv[i])
    {
        printf("%s", argv[i]); // put a space after every word
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (n_flag != 1) // if there are no -n n_flag will be 0
        printf("\n");
    return (0);
}
