/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/09 21:50:26 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void free_map(t_map *map) {
	if (map && map->m) {
		for (int i = 0; i < map->h; i++) {
			free(map->m[i]);
		}
		free(map->m);
	}
}

static int is_valid_char(char c) {
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int is_surrounded_by_walls(t_map *map) {
	for (int i = 0; i < map->h; i++) {
		if (map->m[i][0] != 1 || map->m[i][map->w - 1] != 1)
			return 0;
	}
	for (int j = 0; j < map->w; j++) {
		if (map->m[0][j] != 1 || map->m[map->h - 1][j] != 1)
			return 0;
	}
	return 1;
}

static int parse_map_line(char *line, int *row, int width) {
	for (int j = 0; j < width; j++) {
		if (!is_valid_char(line[j])) {
			printf("Error: Invalid character '%c' in map\n", line[j]);
			return -1;
		}
		if (line[j] == '1')
			row[j] = 1;
		else if (line[j] == '0')
			row[j] = 0;
		else if (line[j] == 'N' || line[j] == 'S' || line[j] == 'E' || line[j] == 'W')
			row[j] = 0; // Player start positions are treated as empty spaces
	}
	return 0;
}

static int count_valid_lines(FILE *file) {
	char line[256];
	int line_count = 0;

	while (fgets(line, sizeof(line), file)) {
		if (strchr(line, '1') || strchr(line, '0') || strchr(line, 'N') || strchr(line, 'S') || strchr(line, 'E') || strchr(line, 'W'))
			line_count++;
	}
	return line_count;
}

static int allocate_map(t_map *map, int line_count) {
	map->h = line_count;
	map->m = (int **)malloc(sizeof(int *) * map->h);
	if (!map->m) {
		perror("Error allocating memory for map");
		return -1;
	}
	return 0;
}

static int read_map_lines(FILE *file, t_map *map) {
	char line[256];
	int i = 0;

	while (fgets(line, sizeof(line), file)) {
		if (strchr(line, '1') || strchr(line, '0') || strchr(line, 'N') || strchr(line, 'S') || strchr(line, 'E') || strchr(line, 'W')) {
			map->w = strlen(line);
			if (line[map->w - 1] == '\n') {
				line[map->w - 1] = '\0';
				map->w--;
			}

			map->m[i] = (int *)calloc(map->w, sizeof(int));
			if (!map->m[i]) {
				perror("Error allocating row memory");
				free_map(map);
				return -1;
			}

			if (parse_map_line(line, map->m[i], map->w) == -1) {
				free_map(map);
				return -1;
			}
			i++;
		}
	}
	return 0;
}

static int init_map(t_map *map, const char *filename) {
	FILE *file = fopen(filename, "r");
	if (!file) {
		perror("Error opening map file");
		return -1;
	}

	int line_count = count_valid_lines(file);
	fseek(file, 0, SEEK_SET);

	if (allocate_map(map, line_count) == -1) {
		fclose(file);
		return -1;
	}

	if (read_map_lines(file, map) == -1) {
		fclose(file);
		return -1;
	}

	fclose(file);

	if (!is_surrounded_by_walls(map)) {
		printf("Error: Map is not surrounded by walls\n");
		free_map(map);
		return -1;
	}

	return 0;
}

static int init_mlx(t_mlx *mlx) {
	mlx->mlx = mlx_init();
	if (!mlx->mlx) {
		printf("Error: mlx_init failed\n");
		return -1;
	}
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win) {
		printf("Error: mlx_new_window failed\n");
		return -1;
	}
	mlx->data.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->data.img) {
		printf("Error: mlx_new_image failed\n");
		return -1;
	}
	mlx->data.addr = mlx_get_data_addr(mlx->data.img, &mlx->data.bits_per_pixel, &mlx->data.line_length, &mlx->data.endian);
	if (!mlx->data.addr) {
		printf("Error: mlx_get_data_addr failed\n");
		return -1;
	}
	return 0;
}

static int init_state(t_state *state) {
	get_dir_v(&state->dir, UP);
	state->pos.x = 5;
	state->pos.y = 5;
	state->pos.x = -1; // Initialize to invalid position
	state->pos.y = -1;
	state->rot = NONE_ROT;
	v_perp(&state->plane, state->dir);
	v_mul(&state->plane, state->plane, tan(FOV / 2));
	return 0;
}

static int init_textures(t_cub3d *cube) {
	int i;
	t_img t;
	char *texts[4] = {
		"./texture/wolfenstein/bluestone.xpm",
		"./texture/wolfenstein/colorstone.xpm",
		"./texture/wolfenstein/eagle.xpm",
		"./texture/wolfenstein/redbrick.xpm",
	};

	for (i = 0; i < 4; i++) {
		t.img = mlx_xpm_file_to_image(cube->mlx.mlx, texts[i], &t.img_width, &t.img_height);
		if (!t.img) {
			printf("Error: Failed to load texture %s\n", texts[i]);
			return -1;
		}
		t.addr = mlx_get_data_addr(t.img, &t.bits_per_pixel, &t.line_length, &t.endian);
		if (!t.addr)
			return -1;
		cube->texture[i] = t;
	}
	cube->ceiling_color = BLUE2;
	cube->floor_color = GREEN;
	return 0;
}

void init_cube(t_cub3d *cube, const char *filename) {
	ft_bzero(cube, sizeof(t_cub3d));
	if (init_map(&cube->map, filename) == -1)
		exit_error(cube, "init_map() failed");
	if (init_state(&cube->state) == -1)
		exit_error(cube, "init_state() failed");
	if (init_mlx(&cube->mlx) == -1)
		exit_error(cube, "mlx_init() failed");
	if (init_textures(cube) == -1)
		exit_error(cube, "init_textures() failed");
}
