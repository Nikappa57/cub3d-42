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

size_t	ft_strcspn(const char *s, const char *reject)
{
	if (!s || !reject)
		return 0; // Check per puntatori nulli

	const char *p = s;
	const char *r;
	size_t count = 0;

	while (*p != '\0')
	{
		r = reject;
		while (*r != '\0')
		{
			if (*p == *r)
				return count;
			r++;
		}
		p++;
		count++;
	}
	return count;
}

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return 0; // Check per puntatori nulli

	int i = 0;

	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

ssize_t	fd_getline(char **line, size_t *len, int fd)
{
	if (!line || !len || fd < 0)
		return -1; // Check input valido

	size_t buffer_size = *len > 0 ? *len : 128;

	if (!*line)
	{
		*line = malloc(buffer_size);
		if (!*line)
			return -1; // Allocazione fallita
	}

	size_t position = 0;
	char c;
	ssize_t bytes_read;

	while ((bytes_read = read(fd, &c, 1)) > 0)
	{
		if (position >= buffer_size - 1)
		{
			// Espansione buffer dinamico
			buffer_size *= 2;
			char *new_line = realloc(*line, buffer_size);
			if (!new_line)
			{
				free(*line);
				*line = NULL;
				return -1; // Allocazione fallita
			}
			*line = new_line;
		}
		if (c == '\n')
			break;
		(*line)[position++] = c;
	}

	if (bytes_read < 0 || (bytes_read == 0 && position == 0))
	{
		free(*line);
		*line = NULL;
		return -1; // Errore di lettura o EOF senza dati
	}

	(*line)[position] = '\0';
	*len = buffer_size;
	return position;
}
