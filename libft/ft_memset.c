/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbournon <tbournon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 20:46:55 by tbournon          #+#    #+#             */
/*   Updated: 2023/05/18 21:01:30 by tbournon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	char			*tmp;
	unsigned char	tmpc;

	i = 0;
	tmp = (char *)b;
	tmpc = (unsigned char)c;
	while (i < len)
	{
		tmp[i] = tmpc;
		i++;
	}
	return (b);
}
