/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 09:56:47 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	memset(ptr, 0, count * size);
	return (ptr);
}

void skip_texture_info(FILE *file)
{
	char line[256];
	int i = 0;
	while (i < 7)
	{
		if (fgets(line, sizeof(line), file) == NULL)
		{
			perror("Error reading texture info");
			exit(1);
		}
		i++;
	}
}

static int	init_mlx(t_mlx *mlx) {
	mlx->mlx = mlx_init();
	if (!mlx->mlx) {
		fprintf(stderr, "Error: Failed to initialize mlx\n");
		return -1;
	}
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win) {
		fprintf(stderr, "Error: Failed to create window\n");
		return -1;
	}
	mlx->data.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->data.img) {
		fprintf(stderr, "Error: Failed to create image\n");
		return -1;
	}
	mlx->data.addr = mlx_get_data_addr(mlx->data.img, &mlx->data.bits_per_pixel, &mlx->data.line_length, &mlx->data.endian);
	if (!mlx->data.addr) {
		fprintf(stderr, "Error: Failed to get data address\n");
		return -1;
	}
	return 0;
}

static int	init_map(t_map *map, const char *map_path)
{
	if (!map || !map_path)
	{
		printf("Error: Invalid map or path\n");
		return -1;
	}

	FILE	*file = fopen(map_path, "r");
	if (!file)
	{
		perror("Error opening map file");
		return -1;
	}

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

static void	set_position_and_direction(t_state *state,
	char direction_char, int col, int row)
{
	state->pos.x = col + 0.5;
	state->pos.y = row + 0.5;
	if (direction_char == 'N')
		get_dir_v(&state->dir, UP);
	else if (direction_char == 'S')
		get_dir_v(&state->dir, DOWN);
	else if (direction_char == 'W')
		get_dir_v(&state->dir, LEFT);
	else if (direction_char == 'E')
		get_dir_v(&state->dir, RIGHT);
}

static void	parse_line(t_state *state, const char *line, int row, int *player_count)
{
	size_t col = 0;
	size_t actual_col = 0;

	while (col < strlen(line))
	{
		if (line[col] == 'N' || line[col] == 'S'
			|| line[col] == 'W' || line[col] == 'E')
		{
			(*player_count)++;
			set_position_and_direction(state, line[col], actual_col, row);
		}
		if (line[col] == '\t')
			actual_col += 4;
		else
			actual_col++;
		col++;
	}
}

static void	parse_player(t_state *state, const char *map_path)
{
	FILE	*file;
	char	line[256];
	int		row;
	int		player_count = 0;

	file = fopen(map_path, "r");

	skip_texture_info(file);

	row = 0;
	while (fgets(line, sizeof(line), file))
	{
		parse_line(state, line, row, &player_count);
		row++;
	}
	if (player_count != 1)
	{
		printf("\033[0;31mError: Invalid number of players\033[0m\n");
		exit(1);
	}
	fclose(file);
}

static int	init_state(t_state *state, const char *map_path)
{
	parse_player(state, map_path);

	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;
	v_perp(&state->plane, state->dir);
	v_mul(&state->plane, state->plane, tan(FOV / 2));
	return (0);
}

void	free_map(t_map *map)
{
	int i = 0;
	while (i < map->h)
	{
		free(map->m[i]);
		i++;
	}
	free(map->m);
}

bool	flood_fill(t_map *map, int x, int y, bool **visited)
{
	if (x < 0 || y < 0 || x >= map->w || y >= map->h)
	{
		printf("\033[0;31mOut of bounds: (%d, %d)\033[0m\n", x, y);
		return false;
	}
	if (map->m[y][x] == 1 || visited[y][x])
	{
		printf("\033[0;32mHit wall or visited: (%d, %d)\033[0m\n", x, y);
		return true;
	}

	visited[y][x] = true;
	printf("Visiting: (%d, %d)\n", x, y);

	bool	up = flood_fill(map, x, y - 1, visited);
	bool	down = flood_fill(map, x, y + 1, visited);
	bool	left = flood_fill(map, x - 1, y, visited);
	bool	right = flood_fill(map, x + 1, y, visited);

	return up && down && left && right;
}

void	skip_spaces(const int *line, int *len)
{
	while (line[*len] == ' ' || line[*len] == '\t')
		(*len)++;
}

bool	is_map_enclosed(t_state *state, t_map *map)
{
	bool	**visited = (bool **)malloc(map->h * sizeof(bool *));
	int		i = 0;

	while (i < map->h)
	{
		visited[i] = (bool *)calloc(map->w, sizeof(bool));
		i++;
	}

	int	player_x = state->pos.x;
	int	player_y = state->pos.y;

	printf("Inizio flood fill da: (%d, %d)\n", player_x, player_y);
	bool	result = flood_fill(map, player_x, player_y, visited);

	i = 0;
	while (i < map->h)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
	return result;
}

static int	parse_color(const char *str) {
	int	r, g, b;
	char	*endptr;

	r = strtol(str, &endptr, 10);
	if (*endptr != ',')
		return -1;
	str = endptr + 1;

	g = strtol(str, &endptr, 10);
	if (*endptr != ',')
		return -1;
	str = endptr + 1;

	b = strtol(str, &endptr, 10);
	if (*endptr != '\0' && *endptr != '\n')
		return -1;
	printf("\033[0;31mColor parsed: %d, %d, %d\033[0m\n", r, g, b);
	return (r << 16) | (g << 8) | b;
}

static int	read_config(const char *file_path, t_config *config) {
	FILE	*file = fopen(file_path, "r");
	if (!file) {
		fprintf(stderr, "Error: Failed to open config file %s\n", file_path);
		return -1;
	}

	char	line[256];
	while (fgets(line, sizeof(line), file)) {
		if (strncmp(line, "NO ", 3) == 0)
			config->north_texture = strdup(line + 3);
		else if (strncmp(line, "SO ", 3) == 0)
			config->south_texture = strdup(line + 3);
		else if (strncmp(line, "WE ", 3) == 0)
			config->west_texture = strdup(line + 3);
		else if (strncmp(line, "EA ", 3) == 0)
			config->east_texture = strdup(line + 3);
		else if (strncmp(line, "F ", 2) == 0)
			config->floor_color = parse_color(line + 2);
		else if (strncmp(line, "C ", 2) == 0)
			config->ceiling_color = parse_color(line + 2);
	}

	fclose(file);
	return 0;
}

static int	load_texture(t_cub3d *cube, t_img *t, const char *path) {
	printf("Attempting to load texture from path: %s\n", path);
	t->img = mlx_xpm_file_to_image(cube->mlx.mlx, (char *)path, &t->img_width, &t->img_height);
	if (!t->img) {
		fprintf(stderr, "Error: Failed to load texture %s\n", path);
		fprintf(stderr, "mlx_xpm_file_to_image returned NULL\n");
		return -1;
	}
	printf("Texture loaded successfully: %s\n", path);
	t->addr = mlx_get_data_addr(t->img, &t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr) {
		fprintf(stderr, "Error: Failed to get data address for texture %s\n", path);
		fprintf(stderr, "mlx_get_data_addr returned NULL\n");
		return -1;
	}
	printf("Data address obtained successfully for texture: %s\n", path);
	return 0;
}

static int	init_textures(t_cub3d *cube, const char *map_path) {
	t_config	config;
	if (read_config(map_path, &config) == -1)
		return -1;

	// Trim newline characters from texture paths
	config.north_texture[strcspn(config.north_texture, "\n")] = '\0';
	config.south_texture[strcspn(config.south_texture, "\n")] = '\0';
	config.west_texture[strcspn(config.west_texture, "\n")] = '\0';
	config.east_texture[strcspn(config.east_texture, "\n")] = '\0';

	printf("North texture path: %s\n", config.north_texture);
	printf("South texture path: %s\n", config.south_texture);
	printf("West texture path: %s\n", config.west_texture);
	printf("East texture path: %s\n", config.east_texture);

	if (load_texture(cube, &cube->texture[0], config.north_texture) == -1 ||
		load_texture(cube, &cube->texture[1], config.south_texture) == -1 ||
		load_texture(cube, &cube->texture[2], config.west_texture) == -1 ||
		load_texture(cube, &cube->texture[3], config.east_texture) == -1)
		return -1;

	cube->ceiling_color = config.ceiling_color;
	cube->floor_color = config.floor_color;

	free(config.north_texture);
	free(config.south_texture);
	free(config.west_texture);
	free(config.east_texture);
	
	return 0;
}

void	init_cube(t_cub3d *cube, const char *map_path)
{
	ft_bzero(cube, sizeof(t_cub3d));
	if (init_map(&cube->map, map_path) == -1)
		exit_error(cube, "init_map() failed");
	if (init_state(&cube->state, map_path) == -1)
		exit_error(cube, "init_state() failed");
	if (!is_map_enclosed(&cube->state, &cube->map))
		exit_error(cube, "Map is not enclosed");
	if (init_mlx(&cube->mlx) == -1)
		exit_error(cube, "init_mlx() failed");
	if (init_mlx(&cube->mlx_test) == -1)
		exit_error(cube, "init_mlx() failed");
	if (init_textures(cube, map_path) == -1)
		exit_error(cube, "init_textures() failed");
}
