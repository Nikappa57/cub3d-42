/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:14:35 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/22 14:39:13 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
