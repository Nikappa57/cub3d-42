/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puts_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 20:34:11 by lorenzogaud       #+#    #+#             */
/*   Updated: 2023/02/19 20:38:55 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putptr_print(unsigned long n)
{
	char	*base;

	base = "0123456789abcdef";
	if (n / 16)
		ft_putptr_print(n / 16);
	ft_putchar_fd(base[n % 16], 1);
}

void	ft_puthex_print(unsigned int n, const char type)
{
	char	*base;

	base = "0123456789abcdef";
	if (n / 16)
		ft_puthex_print(n / 16, type);
	if (type == 'X')
		ft_putchar_fd(ft_toupper(base[n % 16]), 1);
	else
		ft_putchar_fd(base[n % 16], 1);
}

void	ft_putunbr_print(unsigned int n)
{
	if (n / 10)
		ft_putunbr(n / 10);
	ft_putchar(n % 10 + '0');
}
