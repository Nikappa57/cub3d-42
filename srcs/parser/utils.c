/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:14:35 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/20 14:50:36 by lottavi          ###   ########.fr       */
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

void	skip_texture_info(int fd)
{
	char	*line;
	int		is_empty;
	int		i;

	while ((line = get_next_line(fd)) != NULL)
	{
		if (!line) continue; // Add null check
		is_empty = 1;
		i = 0;
		while (line[i] != '\0')
		{
			if (!ft_isspace(line[i]))
			{
				is_empty = 0;
				break;
			}
			i++;
		}
		if (is_empty)
		{
			free(line);
			continue;
		}

		if (ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0 ||
			ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line, "EA", 2) == 0 ||
			ft_strncmp(line, "F", 1) == 0 || ft_strncmp(line, "C", 1) == 0)
		{
			free(line);
			continue;
		}
		else
		{
			lseek(fd, -strlen(line), SEEK_CUR);
			free(line);
			break;
		}
		free(line);
	}
}

bool	flood_fill(t_map *map, int x, int y, bool **visited)
{
	bool	up;
	bool	down;
	bool	left;
	bool	right;

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
	up = flood_fill(map, x, y - 1, visited);
	down = flood_fill(map, x, y + 1, visited);
	left = flood_fill(map, x - 1, y, visited);
	right = flood_fill(map, x + 1, y, visited);
	return (up && down && left && right);
}

void	free_map(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->h)
	{
		free(map->m[i]);
		i++;
	}
	free(map->m);
}

char	*skip_spaces_and_tabs(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}
