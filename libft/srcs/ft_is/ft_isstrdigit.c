/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isstrdigit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 23:18:34 by lorenzogaud       #+#    #+#             */
/*   Updated: 2023/04/13 12:34:18 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check_maxmin(const char *str)
{
	long int	nbr;
	int			sign;

	nbr = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
	{
		str++;
		sign = -1;
	}
	else if (*str == '+')
		str++;
	while (ft_isdigit(*str))
		nbr = (nbr * 10) + (*str++ - '0');
	nbr *= sign;
	if ((nbr > 2147483647) || (nbr < -2147483648))
	{
		return (0);
	}
	return (1);
}

int	ft_isstrdigit(const char *s)
{
	int	i;

	i = 0;
	if ((s[i] == '+' || s[i] == '-') && s[i + 1])
		i++;
	if (ft_strlen(s) > 12)
		return (0);
	while (s[i])
		if (!ft_isdigit(s[i++]))
			return (0);
	return (ft_check_maxmin(s));
}
