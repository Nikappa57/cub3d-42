/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:14:35 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 12:09:28 by lottavi          ###   ########.fr       */
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

int skip_texture_info(int fd)
{
	char *line;
	int is_empty;
	int i;
	int lines_skipped = 0;

	line = get_next_line(fd);
	if (fd < 0)
	{
		printf("Error: Invalid file descriptor\n");
		return (-1);
	}

	while ((line = get_next_line(fd)) != NULL)
	{
		is_empty = true;
		i = 0;

		while (line[i] != '\0')
		{
			if (!ft_isspace(line[i]))
			{
				is_empty = false;
				break;
			}
			i++;
		}

		if (is_empty)
		{
			free(line);
			lines_skipped++;
			continue;
		}

		if (ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0 ||
			ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line, "EA", 2) == 0 ||
			ft_strncmp(line, "F", 1) == 0 || ft_strncmp(line, "C", 1) == 0)
		{
			free(line);
			lines_skipped++;
			continue;
		}
		else
		{
			if (lseek(fd, -strlen(line), SEEK_CUR) == -1)
			{
				printf("Error: Failed to rewind file descriptor\n");
				free(line);
				return (-1);
			}
			free(line);
			break;
		}
	}
	return (printf("\033[0;34m[DEBUG] Skipped %d lines\033[0m\n", lines_skipped), lines_skipped);
}

bool	flood_fill(t_map *map, int x, int y, bool **visited)
{
	if (x < 0 || y < 0 || x >= map->w || y >= map->h)
	{
		printf("\033[0;31mOut of bounds: (%d, %d)\033[0m\n", x, y);
		return (false);
	}
	if (map->m[y][x] == 1 || visited[y][x])
	{
		printf("\033[0;32mHit wall or visited: (%d, %d)\033[0m\n", x, y);
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
