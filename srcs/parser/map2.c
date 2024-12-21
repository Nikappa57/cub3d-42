/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:21:44 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 22:51:31 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static bool	is_texture(char *line)
{
	if (!line)
		return (false);
	return (!ft_strncmp("NO", line, 2) || !ft_strncmp("SO", line, 2)
		|| !ft_strncmp("WE", line, 2) || !ft_strncmp("EA", line, 2)
		|| !ft_strncmp("F", line, 1) || !ft_strncmp("C", line, 1)
		|| !ft_strncmp("\n", line, 1));
}

char	*skip_texture(int fd)
{
	char	*line;
	int		count;

	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	count = 0;
	while (is_texture(line))
	{
		free(line);
		line = get_next_line(fd);
		count++;
	}
	return (line);
}

int	get_map_width(const char *map_path)
{
	int		fd;
	int		width;
	char	*line;
	int		i;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error\n opening map file"), (-1));

	line = skip_texture(fd);
	width = 0;
	while (line)
	{
		i = ft_strlen(line);
		if (i > width)
			width = i;
		free(line);
		line = get_next_line(fd);
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
		return (perror("Error\n opening map file"), (-1));
	height = 0;
	line = skip_texture(fd);
	while (line)
	{
		height++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (height);
}

int	init_map_dimensions(t_map *map, const char *map_path)
{
	map->w = get_map_width(map_path);
	map->h = get_map_height(map_path);
	if (map->w < 1 || map->h < 0)
		return (-1);
	return (0);
}
