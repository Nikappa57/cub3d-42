/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/19 16:08:25 by lottavi          ###   ########.fr       */
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

static int init_map(t_map *map, const char *map_path)
{
	char line[256];
	int width = 0;
	int height = 0;
	FILE *file = fopen(map_path, "r");
	while (fgets(line, sizeof(line), file))
	{
		int len = strlen(line);
		if (len > width)
			width = len;
		height++;
	}
	map->w = width;
	map->h = height;
	map->m = (int **)malloc(sizeof(int *) * height);
	if (!map->m)
	{
		fclose(file);
		return -1;
	}
	rewind(file);
	int row = 0;
	while (fgets(line, sizeof(line), file))
	{
		map->m[row] = calloc(width, sizeof(int));
		if (!map->m[row])
		{
			fclose(file);
			return -1;
		}

		int col = 0;
		for (size_t i = 0; i < strlen(line); i++)
		{
			if (line[i] != '\n')
			{
				if (line[i] == '1')
					map->m[row][col] = 1;
				else if (line[i] == '0')
					map->m[row][col] = 0;
				col++;
			}
		}
		row++;
	}
	fclose(file);
	return 0;
}

static void set_position_and_direction(t_state *state, char direction_char, int col, int row)
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

static void parse_map(t_state *state, const char *map_path)
{
	FILE *file = fopen(map_path, "r");
	char line[256];
	int row = 0;
	while (fgets(line, sizeof(line), file))
	{
		for (size_t col = 0; col < strlen(line); col++)
		{
			if (line[col] == 'N' || line[col] == 'S' || line[col] == 'W' || line[col] == 'E')
			{
				set_position_and_direction(state, line[col], col, row);
				break;
			}
		}
		row++;
	}
	fclose(file);
}

static int init_state(t_state *state, const char *map_path)
{
	parse_map(state, map_path);

	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;
	v_perp(&state->plane, state->dir);
	v_mul(&state->plane, state->plane, tan(FOV / 2));
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

void init_cube(t_cub3d *cube, const char *map_path)
{
	ft_bzero(cube, sizeof(t_cub3d));
	if (init_state(&cube->state, map_path) == -1)
		exit_error(cube, "init_state() failed");
	if (init_map(&cube->map, map_path) == -1)
		exit_error(cube, "init_map() failed");
	if (init_mlx(&cube->mlx) == -1)
		exit_error(cube, "init_mlx() failed");
	if(init_mlx(&cube->mlx_test) == -1)
		exit_error(cube, "init_mlx() failed");
	if (init_textures(cube) == -1)
		exit_error(cube, "init_textures() failed");
}
