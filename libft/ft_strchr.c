/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:27:36 by ssukhija          #+#    #+#             */
/*   Updated: 2025/10/11 22:27:36 by ssukhija         ###   ########.fr       */
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
