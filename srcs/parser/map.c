/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:10:37 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 21:41:31 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	allocate_map_memory(t_map *map, int height, int width)
{
	int	i;
	int	j;

	map->m = (int **)malloc(height * sizeof(int *));
	if (!map->m)
		return (printf("Error: Memory allocation failed\n"), (-1));
	i = 0;
	while (i < height)
	{
		map->m[i] = (int *)calloc(width, sizeof(int));
		if (!map->m[i])
		{
			printf("Error: Memory allocation failed for map row\n");
			j = 0;
			while (j < i)
			{
				free(map->m[j]);
				j++;
			}
			free(map->m);
			return (-1);
		}
		i++;
	}
	return (0);
}

void	process_map(t_map *map, char *line, int row, int width)
{
	int		col;
	size_t	i;

	col = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != '\n')
		{
			if (col < width)
			{
				if (line[i] == '1')
					map->m[row][col++] = 1;
				else
					map->m[row][col++] = 0;
			}
		}
		i++;
	}
	printf("\033[0;34m[DEBUG MALLOC] Processed line: %s\033[0m", line);
}

int	init_map(t_map *map, const char *map_path)
{
	int		fd;
	int		skip_info;
	char	*line;
	int		row;

	if (init_map_dimensions(map, map_path) == -1)
		return (-1);
	printf("\033[0;32m[DEBUG] Map dimensions initialized w:%d, h:%d\033[0m\n", map->w, map->h);
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error opening map file"), -1);
	skip_info = skip_texture_info(fd);
	if (allocate_map_memory(map, map->h, map->w) == -1)
	{
		close(fd);
		return (printf("\033[0;31mError: Allocation failed\033[0m\n"), -1);
	}
	map->h -= skip_info;
	row = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		process_map(map, line, row++, map->w);
		free(line);
	}
	close(fd);
	return (printf("\033[0;32m[DEBUG] Map initialized\033[0m\n"), 0);
}
