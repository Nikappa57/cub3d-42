/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/18 17:53:54 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void *ft_calloc(size_t count, size_t size)
{
	void *ptr = malloc(count * size);
	if (!ptr)
	{
		perror("Error allocating memory");
		return NULL;
	}
	memset(ptr, 0, count * size);
	return ptr;
}

static int is_surrounded_by_walls(t_map *map)
{
	for (int i = 0; i < map->h; i++)
	{
		if (map->m[i][0] != 1 || map->m[i][map->w - 1] != 1)
			return 0;
	}
	for (int j = 0; j < map->w; j++)
	{
		if (map->m[0][j] != 1 || map->m[map->h - 1][j] != 1)
			return 0;
	}
	return 1;
}

int parse_map_line(const char *line, int *row, int len, int i, t_state *state)
{
	for (int j = 0; j < len; j++)
	{
		if (line[j] == '1')
			row[j] = 1;
		else if (line[j] == '0')
			row[j] = 0;
		else if (line[j] == 'N' || line[j] == 'S' || line[j] == 'E' || line[j] == 'W')
		{
			row[j] = 2;
			state->pos.x = j + 0.5;
			state->pos.y = i + 0.5;
			if (line[j] == 'N')
				get_dir_v(&state->dir, UP);
			else if (line[j] == 'S')
				get_dir_v(&state->dir, DOWN);
			else if (line[j] == 'E')
				get_dir_v(&state->dir, RIGHT);
			else if (line[j] == 'W')
				get_dir_v(&state->dir, LEFT);
		}
		else
		{
			fprintf(stderr, "Invalid character '%c' in line %d\n", line[j], i);
			return -1;
		}
	}
	return 0;
}

static int count_valid_lines(FILE *file)
{
	char line[256];
	int line_count = 0;

	while (fgets(line, sizeof(line), file))
	{
		if (strpbrk(line, "10NSEW"))
			line_count++;
	}
	return line_count;
}

static int allocate_map(t_map *map, int line_count)
{
	map->h = line_count;
	map->m = (int **)ft_calloc(map->h, sizeof(int *));
	if (!map->m)
	{
		perror("Error allocating memory for map");
		return -1;
	}
	return 0;
}

static int read_map_lines_pass1(FILE *file, int *max_w)
{
	char line[256];

	while (fgets(line, sizeof(line), file))
	{
		if (strpbrk(line, "10NSEW"))
		{
			int len = strlen(line);
			if (line[len - 1] == '\n')
				line[--len] = '\0';
			while (len > 0 && isspace((unsigned char)line[len - 1]))
				line[--len] = '\0';
			if (len > *max_w)
				*max_w = len;
		}
	}
	return 0;
}

static int read_map_lines_pass2(FILE *file, t_map *map, t_state *state)
{
	char line[256];
	int i = 0;

	while (fgets(line, sizeof(line), file))
	{
		if (strpbrk(line, "10NSEW"))
		{
			int len = strlen(line);
			if (line[len - 1] == '\n')
				line[--len] = '\0';
			while (len > 0 && isspace((unsigned char)line[len - 1]))
				line[--len] = '\0';
			map->m[i] = (int *)ft_calloc(map->w, sizeof(int));
			if (!map->m[i])
			{
				perror("Error allocating memory for map row");
				return -1;
			}
			if (parse_map_line(line, map->m[i], map->w, i, state) == -1)
				return -1;
			i++;
		}
	}
	return 0;
}

int read_map_lines(FILE *file, t_map *map, t_state *state)
{
	int max_w = 0;

	read_map_lines_pass1(file, &max_w);
	if (fseek(file, 0, SEEK_SET) != 0)
	{
		perror("Error resetting file position");
		return -1;
	}
	map->w = max_w;
	return read_map_lines_pass2(file, map, state);
}

static int init_map(t_map *map, t_state *state, const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		perror("Error opening map file");
		return -1;
	}

	int line_count = count_valid_lines(file);
	if (fseek(file, 0, SEEK_SET) != 0)
	{
		perror("Error resetting file position");
		fclose(file);
		return -1;
	}

	if (allocate_map(map, line_count) == -1)
	{
		fclose(file);
		return -1;
	}

	if (read_map_lines(file, map, state) == -1)
	{
		fclose(file);
		return -1;
	}

	if (!is_surrounded_by_walls(map))
	{
		fprintf(stderr, "Error: Map is not surrounded by walls\n");
		fclose(file);
		return -1;
	}

	fclose(file);
	return 0;
}

static int init_mlx(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
	{
		fprintf(stderr, "Error: mlx_init failed\n");
		return -1;
	}
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win)
	{
		fprintf(stderr, "Error: mlx_new_window failed\n");
		return -1;
	}
	mlx->data.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->data.img)
	{
		fprintf(stderr, "Error: mlx_new_image failed\n");
		return -1;
	}
	mlx->data.addr = mlx_get_data_addr(mlx->data.img, &mlx->data.bits_per_pixel, &mlx->data.line_length, &mlx->data.endian);
	if (!mlx->data.addr)
	{
		fprintf(stderr, "Error: mlx_get_data_addr failed\n");
		return -1;
	}
	return 0;
}

static int init_state(t_state *state)
{
	state->pos.x = 0;
	state->pos.y = 0;
	state->dir.x = 0;
	state->dir.y = 0;
	state->plane.x = 0;
	state->plane.y = 0;
	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;
	return 0;
}

static int load_texture(t_cub3d *cube, t_img *t, const char *path)
{
	t->img = mlx_xpm_file_to_image(cube->mlx.mlx, (char *)path, &t->img_width, &t->img_height);
	if (!t->img)
	{
		fprintf(stderr, "Error: Failed to load texture %s\n", path);
		return -1;
	}
	t->addr = mlx_get_data_addr(t->img, &t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr)
		return -1;
	return 0;
}

static int init_textures(t_cub3d *cube)
{
	const char *textures[] = {
		"./texture/wolfenstein/bluestone.xpm",
		"./texture/wolfenstein/colorstone.xpm",
		"./texture/wolfenstein/eagle.xpm",
		"./texture/wolfenstein/redbrick.xpm"
	};

	for (int i = 0; i < 4; i++)
	{
		if (load_texture(cube, &cube->texture[i], textures[i]) == -1)
			return -1;
	}

	cube->ceiling_color = BLUE2;
	cube->floor_color = GREEN;
	return 0;
}

void init_cube(t_cub3d *cube, const char *filename)
{
	ft_bzero(cube, sizeof(t_cub3d));
	if (init_state(&cube->state) == -1)
		exit_error(cube, "init_state() failed");
	if (init_map(&cube->map, &cube->state, filename) == -1)
		exit_error(cube, "init_map() failed");
	if (init_mlx(&cube->mlx) == -1)
		exit_error(cube, "init_mlx() failed");
	if (init_textures(cube) == -1)
		exit_error(cube, "init_textures() failed");
}
