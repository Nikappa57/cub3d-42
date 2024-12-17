/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puts.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 23:52:51 by lorenzogaud       #+#    #+#             */
/*   Updated: 2023/02/19 22:53:49 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putstr(char *str)
{
	if (!str)
		return (ft_putstr("(null)"));
	ft_putstr_fd(str, 1);
	return (ft_strlen(str));
}

int	ft_puthex(unsigned int n, const char type)
{
	ft_puthex_print(n, type);
	return (ft_lenhex(n));
}

int	ft_putptr(unsigned long n)
{
	ft_putstr_fd("0x", 1);
	ft_putptr_print(n);
	return (ft_lenptr(n) + 2);
}

int	ft_putnbr(int n)
{
	char	*nbr;
	int		len;

	nbr = ft_itoa(n);
	if (!nbr)
		return (-1);
	len = ft_putstr(nbr);
	free(nbr);
	return (len);
}

int	ft_putunbr(unsigned int n)
{
	ft_putunbr_print(n);
	return (ft_lenunbr(n));
}
