/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:11 by ssukhija          #+#    #+#             */
/*   Updated: 2024/10/07 11:25:06 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*mylst;

	mylst = malloc(sizeof(t_list));
	if (mylst == NULL)
		return (NULL);
	mylst->content = content;
	mylst->next = NULL;
	return (mylst);
}
