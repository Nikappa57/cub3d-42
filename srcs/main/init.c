/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/17 16:36:49 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void *ft_calloc(size_t count, size_t size) {
	void *ptr = malloc(count * size);
	if (!ptr) {
		perror("Error allocating memory");
		return NULL;
	}
	size_t i = 0;
	while (i < count * size) {
		((unsigned char *)ptr)[i] = 0;
		i++;
	}
	return ptr;
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

int parse_map_line(const char *line, int *row, int len, int i) {
	for (int j = 0; j < len; j++) {
		if (line[j] == '1') {
			row[j] = 1;
		} else if (line[j] == '0') {
			row[j] = 0;
		} else if (line[j] == 'N' || line[j] == 'S' || line[j] == 'E' || line[j] == 'W') {
			// Aggiungi la logica per gestire i caratteri N, S, E, W
			row[j] = 2; // Valore arbitrario, aggiornalo secondo le tue esigenze
			// Puoi anche aggiornare lo stato se necessario
		} else {
			fprintf(stderr, "Carattere non valido '%c' nella riga %d\n", line[j], i);
			return -1;
		}
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
	map->m = (int **)ft_calloc(sizeof(int *), map->h);
	if (!map->m) {
		perror("Error allocating memory for map");
		return -1;
	}

	for (int i = 0; i < map->h; i++) {
		map->m[i] = NULL;
	}


	return 0;
}

int read_map_lines(FILE *file, t_map *map, t_state *state) {
	char line[256];
	int i = 0;
	int max_w = 0;

	// Prima passata: trova la larghezza massima
	while (fgets(line, sizeof(line), file)) {
		if (strpbrk(line, "10NSEW")) {
			char *newline = strchr(line, '\n');
			if (newline)
				*newline = '\0';

			int len = ft_strlen(line);
			while ((len > 0) && ft_isspace((unsigned char)line[len - 1]))
				line[--len] = '\0';

			if (len > max_w)
				max_w = len;
		}
	}

	// Riposiziona il file pointer all'inizio del file
	if (fseek(file, 0, SEEK_SET) != 0) {
		perror("Errore nel riposizionamento del file");
		return -1;
	}
	// Seconda passata: leggi le righe e riempi la mappa
	map->w = max_w;
	if (!map->m) {
		perror("Errore nell'allocazione della mappa");
		return -1;
	}
	while (fgets(line, sizeof(line), file)) {
		if (strpbrk(line, "10NSEW")) {
			char *newline = strchr(line, '\n');
			if (newline)
				*newline = '\0';
			int len = ft_strlen(line);
			while ((len > 0) && ft_isspace((unsigned char)line[len - 1]))
				line[--len] = '\0';
			map->m[i] = (int *)ft_calloc(sizeof(int), map->w);
			if (!map->m[i]) {
				perror("Errore nell'allocazione della riga della mappa");
				return -1;
			}
			if (parse_map_line(line, map->m[i], len, i) == -1) {
				return -1;
			}
			// Riempie gli spazi vuoti con 0
			for (int j = len; j < map->w; j++) {
				map->m[i][j] = 0;
			}
			// Controllo con t_state: verifica della posizione e direzione iniziale
			if (state->pos.x < 0 || state->pos.y < 0 || state->pos.x >= map->w || state->pos.y >= map->h) {
				fprintf(stderr, "Errore: posizione iniziale fuori dai limiti alla riga %d\n", i);
				return -1;
			}
			if (state->dir.x < 0 && state->dir.y < 0) {
				fprintf(stderr, "Errore: direzione non valida alla riga %d\n", i);
				return -1;
			}
			if (state->plane.x < 0 && state->plane.y < 0) {
				fprintf(stderr, "Errore: piano della camera non valido alla riga %d\n", i);
				return -1;
			}
			i++;
		}
	}
	// Controllo finale: verificare che almeno una posizione valida sia stata impostata
	if (i == 0) {
		fprintf(stderr, "Errore: nessuna riga valida trovata nella mappa\n");
		return -1;
	}
	return 0;
}

static int init_map(t_map *map, t_state *state, const char *filename) {
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

	if (read_map_lines(file, map, state) == -1) {
		fclose(file);
		return -1;
	}

	if (!is_surrounded_by_walls(map)) {
		printf("Error: Map is not surrounded by walls\n");
		fclose(file);
		return -1;
	}
	fclose(file);
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
	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;
	v_perp(&state->plane, state->dir);
	v_mul(&state->plane, state->plane, tan(FOV / 2));
	state->pos.x += 0.5;
	state->pos.y += 0.5;
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
	if (init_map(&cube->map, &cube->state, filename) == -1)
		exit_error(cube, "init_map() failed");
	if (init_state(&cube->state) == -1)
		exit_error(cube, "init_state() failed");
	if (init_mlx(&cube->mlx) == -1)
		exit_error(cube, "mlx_init() failed");
	if (init_textures(cube) == -1)
		exit_error(cube, "init_textures() failed");
}
