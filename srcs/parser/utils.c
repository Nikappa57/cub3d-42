/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:14:35 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 15:07:39 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	memset(ptr, 0, count * size);
	return (ptr);
}

int	skip_texture_info(int fd)
{
	char *line;
	int count = 0;

	while ((line = get_next_line(fd)) != NULL)
	{
		if (line[0] == '\n')
		{
			free(line);
			break;
		}
		free(line);
		count++;
	}
	printf("\033[0;34m[DEBUG SKIP] Skipped %d lines of texture info\033[0m\n", count);
	return count;
}

bool	flood_fill(t_map *map, int x, int y, bool **visited)
{
	if (x < 0 || y < 0 || x >= map->w || y >= map->h)
	{
		printf("\033[0;31[DEBUG FF]mOut of bounds: (%d, %d)\033[0m\n", x, y);
		return (false);
	}
	if (map->m[y][x] == 1 || visited[y][x])
	{
		printf("\033[0;32m[DEBUG FF]Hit wall or visited: (%d, %d)\033[0m\n", x, y);
		return (true);
	}

	visited[y][x] = true;
	printf("Visiting: (%d, %d)\n", x, y);

	// Perform flood fill in all four directions
	return (
		flood_fill(map, x, y - 1, visited) && // Up
		flood_fill(map, x, y + 1, visited) && // Down
		flood_fill(map, x - 1, y, visited) && // Left
		flood_fill(map, x + 1, y, visited)    // Right
	);
}

void	free_map(t_map *map)
{
	if (!map || !map->m)
		return;

	for (int i = 0; i < map->h; i++)
	{
		if (map->m[i])
			free(map->m[i]);
	}
	free(map->m);
	map->m = NULL;
}

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

int ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
