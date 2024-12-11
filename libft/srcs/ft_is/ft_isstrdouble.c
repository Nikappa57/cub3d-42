/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isstrdouble.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 23:18:34 by lorenzogaud       #+#    #+#             */
/*   Updated: 2023/04/19 13:30:25 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isstrdouble(const char *s)
{
	int	i;
	int	dot;

	i = 0;
	dot = 0;
	if ((s[i] == '+' || s[i] == '-') && s[i + 1])
		i++;
	while (s[i])
	{
		if (s[i] == '.')
		{
			if (dot)
				return (0);
			dot = 1;
		}
		else if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}
