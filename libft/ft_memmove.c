/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 13:08:08 by ssukhija          #+#    #+#             */
/*   Updated: 2024/10/08 09:03:18 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*tmp_src;
	char	*tmp_dest;
	size_t	i;

	i = 0;
	if (src == NULL && dest == NULL)
		return (NULL);
	tmp_dest = (char *) dest;
	tmp_src = (char *) src;
	if (tmp_dest > tmp_src)
	{
		while (n-- > 0)
			tmp_dest[n] = tmp_src[n];
	}
	else
	{
		while (i++ < n)
			(*tmp_dest++) = (*tmp_src++);
	}
	return (dest);
}
