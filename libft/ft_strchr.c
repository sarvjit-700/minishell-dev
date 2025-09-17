/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:52:54 by marvin            #+#    #+#             */
/*   Updated: 2024/09/24 10:52:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char			x;
	size_t			n;
	unsigned int	i;

	x = (char) c;
	i = 0;
	n = ft_strlen(s);
	if (x == 0)
		return ((char *) &s[n]);
	else
	{
		while (s[i] != '\0')
		{
			if (s[i] == x)
				return ((char *) &s[i]);
			i++;
		}
	}
	return (NULL);
}
