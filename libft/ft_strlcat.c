/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 11:13:47 by marvin            #+#    #+#             */
/*   Updated: 2024/09/23 11:13:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	maxsize;

	src_len = ft_strlen(src);
	dst_len = ft_strlen(dst);
	maxsize = size - dst_len - 1;
	if (dst_len >= size)
	{
		dst_len = size;
		return (dst_len + src_len);
	}
	if (size > (dst_len + src_len + 1))
		ft_memcpy(dst + dst_len, src, src_len + 1);
	else
	{
		ft_memcpy(dst + dst_len, src, maxsize);
		dst[size - 1] = '\0';
	}
	return (dst_len + src_len);
}
