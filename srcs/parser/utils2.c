/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:03:36 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 19:03:50 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	*skip_spaces_and_tabs(char *str)
{
	if (!str)
		return (NULL);
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

size_t	ft_strcspn(const char *str1, const char *str2)
{
	const char	*s1;
	const char	*s2;

	s1 = str1;
	while (*s1 != '\0')
	{
		s2 = str2;
		while (*s2 != '\0')
		{
			if (*s1 == *s2)
				return (s1 - str1);
			s2++;
		}
		s1++;
	}
	return (s1 - str1);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
