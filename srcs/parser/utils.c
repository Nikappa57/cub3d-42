/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:14:35 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/22 14:52:40 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	*ft_strtok(char *str, const char *delimiters)
{
	static char	*next;
	char		*token_start;

	if (str == NULL)
		str = next;
	if (str == NULL)
		return (NULL);
	while (*str && ft_strchr(delimiters, *str))
		str++;
	if (*str == '\0')
	{
		next = NULL;
		return (NULL);
	}
	token_start = str;
	while (*str && !ft_strchr(delimiters, *str))
		str++;
	if (*str)
	{
		*str = '\0';
		next = str + 1;
	}
	else
		next = NULL;
	return (token_start);
}

int	skip_texture_info(int fd)
{
	char	*line;
	int		count;

	count = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (line[0] == '\n')
		{
			free(line);
			break ;
		}
		free(line);
		line = get_next_line(fd);
		count++;
	}
	printf("\033[0;34m[DEBUG SKIP] Skipped %d lines of Tinfo\033[0m\n", count);
	return (count);
}

bool	flood_fill(t_map *map, int x, int y, bool **visited)
{
	if (x < 0 || y < 0 || x >= map->w || y >= map->h || map->m[y][x] == -1)
	{
		printf("\033[0;31[DEBUG FF]cehck: (%d, %d)\033[0m\n", map->w, map->h);
		printf("\033[0;31[DEBUG FF]Out of bounds: (%d, %d)\033[0m\n", x, y);
		return (false);
	}
	if (map->m[y][x] == 1 || visited[y][x])
	{
		printf("\033[0;32m[DEBUG FF]Passed: (%d, %d)\033[0m\n", x, y);
		return (true);
	}
	visited[y][x] = true;
	printf("Visiting: (%d, %d)\n", x, y);
	return (
		flood_fill(map, x, y - 1, visited)
		&& flood_fill(map, x, y + 1, visited)
		&& flood_fill(map, x - 1, y, visited)
		&& flood_fill(map, x + 1, y, visited)
	);
}

void	free_map(t_map *map)
{
	int	i;

	if (!map || !map->m)
		return ;
	i = 0;
	while (i < map->h)
	{
		if (map->m[i])
			free(map->m[i]);
		i++;
	}
	free(map->m);
	map->m = NULL;
}
