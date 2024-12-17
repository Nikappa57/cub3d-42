/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lenhex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:39:54 by lorenzogaud       #+#    #+#             */
/*   Updated: 2023/03/23 11:39:56 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_lenhex(unsigned int n)
{
	int	len;

	if (!n)
		return (1);
	len = 0;
	while (n)
	{
		len++;
		n /= 16;
	}
	return (len);
}
