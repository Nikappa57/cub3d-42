/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:10:37 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 17:51:44 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	get_map_width(const char *map_path)
{
	int		fd;
	int		width;
	char	*line;
	size_t	i;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error opening map file"), (-1));
	line = get_next_line(fd);
	width = 0;
	if (line)
	{
		i = 0;
		while (line[i] != '\0')
		{
			if (line[i] == '\t')
				width += 4;
			else if (line[i] != '\n')
				width++;
			i++;
		}
		free(line);
	}
	close(fd);
	return (width);
}

int	get_map_height(const char *map_path)
{
	int		fd;
	char	*line;
	int		height;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error opening map file"), (-1));
	height = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		height++;
		free(line);
	}
	close(fd);
	return (height);
}

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
		if (line[i] == '\t')
			col += (col + 4 < width) ? 4 : (width - col);
		else if (line[i] != '\n')
			if (col < width)
				map->m[row][col++] = (line[i] == '1') ? 1 : 0;
		i++;
	}
	printf("\033[0;34m[DEBUG MALLOC] Processed line: %s\033[0m", line);
}

int init_map(t_map *map, const char *map_path)
{
	int		fd;
	int		width;
	int		height;
	int		skip_info;
	char	*line;
	int		row;

	width = get_map_width(map_path);
	height = get_map_height(map_path);
	if (width == -1 || height == -1)
		return -1;
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error opening map file"), -1);
	skip_info = skip_texture_info(fd);
	if (allocate_map_memory(map, height, width) == -1)
	{
		close(fd);
		return (printf("\033[0;31mError: Memory allocation failed\033[0m\n"), -1);
	}
	map->w = width;
	map->h = height - skip_info;
	row = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		process_map(map, line, row++, width);
		free(line);
	}
	close(fd);
	return (printf("\033[0;32m[DEBUG] Map initialized successfully\033[0m\n"), 0);
}
