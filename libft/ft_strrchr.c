/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 21:05:23 by marvin            #+#    #+#             */
/*   Updated: 2024/09/24 21:05:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char			*ans;
	char			x;
	unsigned int	i;

	i = 0;
	x = (char) c;
	ans = NULL;
	while (s[i] != '\0')
	{
		if (x == 0)
			ans = ((char *) &s[i]);
		else
		{
			if (s[i] == x)
				ans = ((char *) &s[i]);
		}
		i++;
	}
	if (s[i] == x)
		ans = (char *) &s[i];
	return (ans);
}
