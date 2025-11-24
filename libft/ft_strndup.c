/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:43:06 by ssukhija          #+#    #+#             */
/*   Updated: 2025/11/20 16:43:06 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*copy;

	copy = malloc(sizeof(char) * (n + 1));
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s, n);
	copy[n] = '\0';
	return (copy);
}
