/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lenptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:38:51 by lorenzogaud       #+#    #+#             */
/*   Updated: 2023/03/23 11:38:59 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_lenptr(unsigned long n)
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
