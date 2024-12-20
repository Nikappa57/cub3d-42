/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 18:59:51 by marvin            #+#    #+#             */
/*   Updated: 2024/12/20 18:59:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include <stddef.h>

size_t	ft_strcspn(const char *s, const char *reject)
{
	if (!s || !reject) return 0; // Add null check
	const char *p = s;
	const char *r;
	size_t count = 0;

	while (*p != '\0') {
		r = reject;
		while (*r != '\0') {
			if (*p == *r) {
				return count;
			}
			r++;
		}
		p++;
		count++;
	}
	return count;
}

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2) return 0; // Add null check
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}
