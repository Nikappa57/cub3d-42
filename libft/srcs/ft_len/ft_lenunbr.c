/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lenunbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:40:16 by lorenzogaud       #+#    #+#             */
/*   Updated: 2023/03/23 11:40:20 by lorenzogaud      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_lenunbr(unsigned int nbr)
{
	int	len;

	if (!nbr)
		return (1);
	len = 0;
	while (nbr)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}
