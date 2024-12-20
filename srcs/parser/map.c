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

int init_map(t_map *map, const char *map_path) {
    if (!map || !map_path) {
        printf("Error: Invalid map or path\n");
        return -1;
    }

    int fd = open(map_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening map file");
        return -1;
    }

    skip_texture_info(fd);

    char *line = NULL;
    size_t len = 1024; // Lunghezza massima della riga
    ssize_t read_bytes;
    int width = 0, height = 0;

    // Calcola dimensioni della mappa
    while ((read_bytes = fd_getline(&line, &len, fd)) != -1) {
        if (read_bytes == -2) {
            printf("Error: Line exceeds buffer size\n");
            free(line);
            close(fd);
            return -1;
        }

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
    }

    free(line);
    close(fd);

    map->w = width;
    map->h = height;

    map->m = (int **)malloc(height * sizeof(int *));
    if (!map->m) {
        printf("Error: Memory allocation failed for map matrix\n");
        return -1;
    }

    for (int i = 0; i < height; i++) {
        map->m[i] = (int *)ft_calloc(width, sizeof(int));
        if (!map->m[i]) {
            printf("Error: Memory allocation failed for map row\n");
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
    line = malloc(len);
    if (!line) {
        printf("Error: Memory allocation failed for line buffer\n");
        close(fd);
        return -1;
    }

    while ((read_bytes = fd_getline(&line, &len, fd)) != -1) {
        if (read_bytes == -2) {
            printf("Error: Line exceeds buffer size\n");
            free(line);
            close(fd);
            return -1;
        }

        int col = 0;
        for (size_t i = 0; line[i] != '\0'; i++) {
            if (line[i] == '\t') {
                for (int j = 0; j < 4 && col < width; j++) {
                    map->m[row][col++] = 0;
                }
            } else if (line[i] != '\n') {
                if (col < width) {
                    map->m[row][col++] = (line[i] == '1') ? 1 : 0;
                }
            }
        }
        row++;
    }

    free(line);
    close(fd);

    return 0;
}
