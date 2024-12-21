/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/21 17:36:47 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	cleanup_resources(t_cub3d *cube)
{
	int	i;

	if (cube->mlx.mlx)
	{
		if (cube->mlx.data.img)
			mlx_destroy_image(cube->mlx.mlx, cube->mlx.data.img);
		if (cube->mlx.win)
			mlx_destroy_window(cube->mlx.mlx, cube->mlx.win);
		i = 0;
		while (i < 4)
		{
			if (cube->texture[i].img)
				mlx_destroy_image(cube->mlx.mlx, cube->texture[i].img);
			i++;
		}
		mlx_destroy_display(cube->mlx.mlx);
		free(cube->mlx.mlx);
	}
	cube->mlx.mlx = NULL;
	cube->mlx.win = NULL;
	cube->mlx.data.img = NULL;
}

int	init_mlx(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (printf("Error: Failed to initialize mlx\n"), (-1));
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win)
		return (printf("Error: Failed to create window\n"), (-1));
	mlx->data.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->data.img)
		return (printf("Error: Failed to create image\n"), (-1));
	mlx->data.addr = mlx_get_data_addr(mlx->data.img,
			&mlx->data.bits_per_pixel,
			&mlx->data.line_length, &mlx->data.endian);
	if (!mlx->data.addr)
		return (printf("Error: Failed to get data address\n"), (-1));
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
	if (init_textures(cube, map_path) == -1)
	{
		cleanup_resources(cube);
		exit_error(cube, "init_textures() failed");
	}
}
