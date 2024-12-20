/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:10:37 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/20 11:20:45 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	init_map(t_map *map, const char *map_path)
{
	FILE	*file;

	if (!map || !map_path)
		return (printf("Error: Invalid map or path\n"), (-1));
	file = fopen(map_path, "r");
	if (!file)
		return (perror("Error opening map file"), (-1));
	skip_texture_info(file);
	char	line[256];
	int		width = 0, height = 0;
	while (fgets(line, sizeof(line), file))
	{
		int	len = 0;
		for (size_t i = 0; i < strlen(line); i++)
		{
			len += (line[i] == '\t') ? 4 : 1;
		}
		if (line[len - 1] == '\n')
			len--;
		if (len > width)
			width = len;
		height++;
	}
	fclose(file);

	map->w = width;
	map->h = height;

	map->m = (int **)malloc(height * sizeof(int *));
	if (!map->m)
	{
		printf("Error: Memory allocation failed for map matrix\n");
		return -1;
	}

	int i = 0;
	while (i < height)
	{
		map->m[i] = (int *)calloc(width, sizeof(int));
		if (!map->m[i])
		{
			printf("Error: Memory allocation failed for map row\n");
			for (int j = 0; j < i; j++)
				free(map->m[j]);
			free(map->m);
			return -1;
		}
		i++;
	}

	file = fopen(map_path, "r");
	if (!file)
	{
		perror("Error reopening map file");
		for (int i = 0; i < height; i++)
			free(map->m[i]);
		free(map->m);
		return -1;
	}

	skip_texture_info(file);

	int	row = 0;
	while (fgets(line, sizeof(line), file))
	{
		int col = 0;
		size_t i = 0;
		while (i < strlen(line))
		{
			if (line[i] == '\t')
			{
				int j = 0;
				while (j < 4)
				{
					map->m[row][col++] = 0;
					j++;
				}
			}
			else if (line[i] != '\n')
			{
				map->m[row][col++] = (line[i] == '1') ? 1 : 0;
			}
			i++;
		}
		row++;
	}
	fclose(file);
	return 0;
}
