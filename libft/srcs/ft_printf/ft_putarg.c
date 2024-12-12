/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putarg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 19:49:36 by lorenzogaud       #+#    #+#             */
/*   Updated: 2023/02/19 22:54:06 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putarg(const char type, va_list *ap)
{
	if (type == 'c')
		return (ft_putchar(va_arg(*ap, int)));
	else if (type == 's')
		return (ft_putstr(va_arg(*ap, char *)));
	else if (type == 'p')
		return (ft_putptr(va_arg(*ap, unsigned long)));
	else if (type == 'd' || type == 'i')
		return (ft_putnbr(va_arg(*ap, int)));
	else if (type == 'u')
		return (ft_putunbr(va_arg(*ap, unsigned int)));
	else if (type == 'x' || type == 'X')
		return (ft_puthex(va_arg(*ap, unsigned int), type));
	else if (type == '%')
		return (ft_putchar('%'));
	return (0);
}
