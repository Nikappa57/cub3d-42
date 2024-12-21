/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:21:44 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 19:31:05 by lottavi          ###   ########.fr       */
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

int	init_map_dimensions(t_map *map, const char *map_path)
{
	map->w = get_map_width(map_path);
	map->h = get_map_height(map_path);
	if (map->w == -1 || map->h == -1)
		return (-1);
	return (0);
}
