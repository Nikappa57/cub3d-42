/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:10:37 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 15:02:36 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int get_map_width(const char *map_path)
{
	int fd = open(map_path, O_RDONLY);
	if (fd < 0) {
		perror("Error opening map file");
		return -1;
	}

	char *line = get_next_line(fd);
	int width = 0;

	if (line) {
		for (size_t i = 0; line[i] != '\0'; i++) {
			if (line[i] == '\t') {
				width += 4;
			} else if (line[i] != '\n') {
				width++;
			}
		}
		free(line);
	}
	close(fd);
	return width;
}

int get_map_height(const char *map_path) {
	int fd = open(map_path, O_RDONLY);
	if (fd < 0) {
		perror("Error opening map file");
		return -1;
	}

	char *line;
	int height = 0;

	while ((line = get_next_line(fd)) != NULL) {
		height++;
		free(line);
	}
	close(fd);
	return height;
}

int allocate_map_memory(t_map *map, int height, int width) {
	map->m = (int **)malloc(height * sizeof(int *));
	if (!map->m) {
		fprintf(stderr, "Error: Memory allocation failed for map matrix\n");
		return -1;
	}

	for (int i = 0; i < height; i++) {
		map->m[i] = (int *)calloc(width, sizeof(int));
		if (!map->m[i]) {
			fprintf(stderr, "Error: Memory allocation failed for map row\n");
			for (int j = 0; j < i; j++) {
				free(map->m[j]);
			}
			free(map->m);
			return -1;
		}
	}
	return 0;
}

void process_map(t_map *map, char *line, int row, int width) {
	int col = 0;
	size_t i = 0;

	while (line[i] != '\0') {
		if (line[i] == '\t') {
			col += (col + 4 < width) ? 4 : (width - col);
		} else if (line[i] != '\n') {
			if (col < width) {
				map->m[row][col++] = (line[i] == '1') ? 1 : 0;
			}
		}
		i++;
	}
	printf("\033[0;34m[DEBUG MALLOC] Processed line: %s\033[0m", line);
}

int init_map(t_map *map, const char *map_path) {
	int fd;
	int width;
	int height;
	int skip_info;

	width = get_map_width(map_path);
	height = get_map_height(map_path);

	if (width == -1 || height == -1) {
		return -1;
	}

	fd = open(map_path, O_RDONLY);
	if (fd < 0) {
		return (perror("Error opening map file"), -1);
	}

	skip_info = skip_texture_info(fd);

	if (allocate_map_memory(map, height, width) == -1) {
		close(fd);
		return (printf("\033[0;31mError: Memory allocation failed\033[0m\n"), -1);
	}

	map->w = width;
	map->h = height - skip_info;

	char *line;
	int row = 0;
	while ((line = get_next_line(fd)) != NULL) {
		process_map(map, line, row++, width);
		free(line);
	}

	close(fd);
	return (printf("\033[0;32m[DEBUG] Map initialized successfully\033[0m\n"), 0);
}
