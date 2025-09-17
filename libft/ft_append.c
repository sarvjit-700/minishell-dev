/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:15:03 by ssukhija          #+#    #+#             */
/*   Updated: 2025/07/29 13:15:03 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_append(char *txt, char *buff)
{
	char	*str;
	int		len;

	if (txt == NULL)
		len = 0;
	else
		len = ft_strlen(txt);
	str = malloc(((len + ft_strlen(buff)) + 1) * sizeof(char));
	if (str == NULL)
	{
		free(txt);
		return (NULL);
	}
	str = ft_strcpy(str, txt, buff);
	free(txt);
	return (str);
}
