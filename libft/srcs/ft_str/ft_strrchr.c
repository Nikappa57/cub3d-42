/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 10:23:09 by lgaudino          #+#    #+#             */
/*   Updated: 2023/02/01 12:35:10 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s);
	while (42)
	{
		if (s[len] == (char)c)
		{
			return ((char *)(s + len));
		}
		if (len == 0)
			break ;
		len--;
	}
	return (0);
}
