/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 11:52:27 by marvin            #+#    #+#             */
/*   Updated: 2024/10/05 11:52:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_len(long nbr)
{
	int		count;

	count = 0;
	if (nbr < 0)
	{
		count = 1;
		nbr = -nbr;
	}
	if (nbr == 0)
		count++;
	while (nbr != 0)
	{
		nbr /= 10;
		count++;
	}
	return (count);
}

static char	*ft_do_work(char *result, long nbr, int num_len)
{
	if (result == NULL)
		return (NULL);
	if (nbr == 0)
		result[0] = '0';
	else if (nbr < 0)
	{
		result[0] = '-';
		nbr = -nbr;
	}
	while (nbr != 0)
	{
		result[num_len - 1] = ((nbr % 10) + 48);
		nbr = nbr / 10;
		num_len--;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		num_len;
	int		keep_len;
	long	nbr;

	nbr = n;
	num_len = get_len(nbr);
	keep_len = num_len;
	result = malloc((num_len + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	ft_do_work(result, nbr, num_len);
	result[keep_len] = 0;
	return (result);
}
