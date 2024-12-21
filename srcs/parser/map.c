/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:10:37 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 10:51:46 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int init_map(t_map *map, const char *map_path) {
    if (!map || !map_path) {
        fprintf(stderr, "Error: Invalid map or path\n");
        return -1;
    }

    int fd = open(map_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening map file");
        return -1;
    }

    skip_texture_info(fd);
    char *line = NULL;
    int width = 0, height = 0;

    // Calcola dimensioni della mappa
    while ((line = get_next_line(fd)) != NULL) {
        int line_length = 0;
        for (size_t i = 0; i < ft_strlen(line); i++) {
            line_length += (line[i] == '\t') ? 4 : 1;
        }
        if (line[line_length - 1] == '\n') {
            line_length--;
        }
        if (line_length > width) {
            width = line_length;
        }
        height++;
        free(line);
    }

    close(fd);

    map->w = width;
    map->h = height;

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

    fd = open(map_path, O_RDONLY);
    if (fd == -1) {
        perror("Error reopening map file");
        for (int i = 0; i < height; i++) {
            free(map->m[i]);
        }
        free(map->m);
        return -1;
    }

    skip_texture_info(fd);
    int row = 0;

    while ((line = get_next_line(fd)) != NULL) {
        int col = 0;
        for (size_t i = 0; line[i] != '\0'; i++) {
            if (line[i] == '\t') {
                col += (col + 4 < width) ? 4 : (width - col);
            } else if (line[i] != '\n') {
                if (col < width) {
                    map->m[row][col++] = (line[i] == '1') ? 1 : 0;
                }
            }
        }
        free(line);
        row++;
    }

    close(fd);
    return 0;
}
