/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atodb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 19:52:39 by lorenzogaud       #+#    #+#             */
/*   Updated: 2024/04/04 16:17:29 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_pow10(unsigned int exp)
{
	if (exp == 1)
		return (10);
	if (exp == 0)
		return (1);
	return (10 * ft_pow10(exp - 1));
}

double	ft_atodb_helper(char *str, int i)
{
	int		dot;
	double	result;

	dot = 0;
	result = 0;
	while (str[i])
	{
		if ((str[i] == '.') || (str[i] == ','))
		{
			if (dot)
				break ;
			dot = 1;
		}
		else if (!ft_isdigit(str[i]))
			break ;
		else if (!dot)
			result = (result * 10) + (double)(str[i] - '0');
		else
			result += (double)(str[i] - '0') / (double)ft_pow10(dot++);
		i++;
	}
	return (result);
}

double	ft_atodb(char *str)
{
	int		sign;
	int		i;

	i = 0;
	sign = 1;
	if (str[i] == '-')
		sign = -1;
	if ((str[i] == '+') || (str[i] == '-'))
		i++;
	return (ft_atodb_helper(str, i) * sign);
}
