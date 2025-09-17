/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:57:27 by marvin            #+#    #+#             */
/*   Updated: 2024/10/03 11:57:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*new_str;
	size_t	i;
	size_t	len_str;

	i = 0;
	while (s1[i] && ft_strchr(set, s1[i]) != NULL)
		i++;
	len_str = ft_strlen(s1);
	while (len_str > i && ft_strchr(set, s1[len_str - 1]) != NULL)
		len_str--;
	new_str = (char *)malloc(len_str - i + 1 * sizeof(char));
	if (new_str == NULL)
		return (NULL);
	ft_strlcpy(new_str, &s1[i], len_str - i + 1);
	new_str[len_str - i] = '\0';
	return (new_str);
}
