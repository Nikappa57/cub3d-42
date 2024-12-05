/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/05 19:32:50 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	init_mlx(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (-1);
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win)
		return (-1);
	mlx->data.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->data.img)
		return (-1);
	mlx->data.addr = mlx_get_data_addr(mlx->data.img, &mlx->data.bits_per_pixel, &mlx->data.line_length,
		&mlx->data.endian);
	if (!mlx->data.addr)
		return (-1);
	return (0);
}

// TODO: parser
static int	init_map(t_map *map)
{
	map->w = 10;
	map->h = 10;
	map->m = (int **) malloc(sizeof(int *) * map->h);
	if (!map->m)
		return (-1);
	for (int i = 0; i < map->h; i++)
	{
		map->m[i] = calloc(map->w, sizeof(int));
		if (!map->m[i])
			return (-1);
		for (int j = 0; j < map->w; j++)
		{
			if ((i == 0) || (i == map->h - 1)
				|| (j == 0) || (j == map->w - 1))
				map->m[i][j] = 1;
		}
	}
	map->m[3][3] = 1;

	return (0);
}

static int	init_state(t_state *state)
{
	// TODO: dir from parser
	get_dir_v(&state->dir, UP);
	// TODO: map pos from parser
	state->pos.x = 5;
	state->pos.y = 5;
	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;
	v_perp(&state->plane, state->dir);
	v_mul(&state->plane, state->plane, tan(FOV / 2));
	return (0);
}

static int	init_textures(t_cub3d *cube)
{
	int		i;
	t_img	t;

	i = -1;
	while (++i < 4)
	{
		t.img = mlx_xpm_file_to_image(cube->mlx.mlx, "./texture/wolfenstein/bluestone.xpm",
			&t.img_width, &t.img_height);
		if (t.img == NULL)
			return (-1);
		t.addr = mlx_get_data_addr(t.img, &t.bits_per_pixel, &t.line_length, &t.endian);
		if (t.addr == NULL)
			return (-1);
		cube->texture[i] = t;
	}
	cube->ceiling_color = BLUE2;
	cube->floor_color = GREEN;
	return (0);
}

void init_cube(t_cub3d *cube)
{
	ft_bzero(cube, sizeof(t_cub3d));
	// TODO: PARSER!!!
	if (init_map(&cube->map) == -1)
		exit_error(cube, "init_map() failed");
	if (init_state(&cube->state) == -1)
		exit_error(cube, "init_state() failed");
	if (init_mlx(&cube->mlx) == -1)
		exit_error(cube, "mlx_init() failed");
	if (init_textures(cube) == -1)
		exit_error(cube, "init_textures() failed");
	if (init_mlx(&cube->mlx_test) == -1)
		exit_error(cube, "mlx_init() failed");
}