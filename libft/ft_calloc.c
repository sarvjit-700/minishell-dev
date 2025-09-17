/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:23:13 by marvin            #+#    #+#             */
/*   Updated: 2024/10/02 16:23:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*tmp;
	size_t			i;
	size_t			max_size;

	max_size = (size_t) - 1;
	if (nmemb != 0 && size > max_size / nmemb)
		return (NULL);
	i = 0;
	tmp = malloc(nmemb * size);
	if (tmp == NULL)
		return (NULL);
	while (i < (nmemb * size))
	{
		tmp[i] = 0;
		i++;
	}
	return (tmp);
}
