/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbournon <tbournon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 14:23:31 by tbournon          #+#    #+#             */
/*   Updated: 2023/08/30 10:33:46 by tbournon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned long	ft_pointer(unsigned long ptr)
{
	int	count;

	count = 0;
	if (ptr >= 16)
	{
		count += ft_pointer(ptr / 16);
		count += ft_pointer(ptr % 16);
	}
	if (ptr < 16)
	{
		if (ptr < 10)
			count += ft_putchar(ptr + 48);
		else
			count += ft_putchar(ptr + 87);
	}
	return (count);
}
