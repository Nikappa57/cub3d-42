/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/19 16:43:03 by lottavi          ###   ########.fr       */
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

static int	init_mlx(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (printf("Error: mlx_init failed\n"), (-1));
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win)
		return (printf("Error: mlx_new_window failed\n"), (-1));
	mlx->data.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->data.img)
		return (printf("Error: mlx_new_image failed\n"), (-1));
	mlx->data.addr = mlx_get_data_addr(mlx->data.img,
			&mlx->data.bits_per_pixel, &mlx->data.line_length,
			&mlx->data.endian);
	if (!mlx->data.addr)
		return (printf("Error: mlx_get_data_addr failed\n"), (-1));
	return (0);
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

static int	load_texture(t_cub3d *cube, t_img *t, const char *path)
{
	t->img = mlx_xpm_file_to_image(cube->mlx.mlx, (char *)path, &t->img_width, &t->img_height);
	if (!t->img)
	{
		fprintf(stderr, "Error: Failed to load texture %s\n", path);
		return (-1);
	}
	t->addr = mlx_get_data_addr(t->img, &t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr)
		return (-1);
	return (0);
}

static int	init_textures(t_cub3d *cube)
{
	int	i;

	const char	*textures[] = {
		"./texture/wolfenstein/bluestone.xpm",
		"./texture/wolfenstein/colorstone.xpm",
		"./texture/wolfenstein/eagle.xpm",
		"./texture/wolfenstein/redbrick.xpm"
	};
	i = 0;
	while (i < 4)
	{
		if (load_texture(cube, &cube->texture[i], textures[i]) == -1)
			return (-1);
		i++;
	}
	cube->ceiling_color = BLUE2;
	cube->floor_color = GREEN;
	return (0);
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
	if (init_textures(cube) == -1)
		exit_error(cube, "init_textures() failed");
}
