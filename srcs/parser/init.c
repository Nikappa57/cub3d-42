/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/22 00:12:34 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	init_mlx(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (printf("Error\n Failed to initialize mlx\n"), (-1));
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win)
		return (printf("Error\n Failed to create window\n"), (-1));
	mlx->data.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->data.img)
		return (printf("Error\n Failed to create image\n"), (-1));
	mlx->data.addr = mlx_get_data_addr(mlx->data.img,
			&mlx->data.bits_per_pixel,
			&mlx->data.line_length, &mlx->data.endian);
	if (!mlx->data.addr)
		return (printf("Error\n Failed to get data address\n"), (-1));
	return (0);
}

void	init_cube(t_cub3d *cube, const char *map_path)
{
	ft_bzero(cube, sizeof(t_cub3d));
	if (validate_cub_file(map_path) == 0)
		exit_error(cube, "Error\nInvalid .cub file syntax");
	if (init_map(&cube->map, map_path) == -1)
		exit_error(cube, "Error\ninit_map() failed");
	if (init_state(&cube->state, map_path) == -1)
		exit_error(cube, "Error\ninit_state() failed");
	if (!is_map_enclosed(&cube->state, &cube->map))
		exit_error(cube, "Error\nMap is not enclosed");
	if (init_mlx(&cube->mlx) == -1)
		exit_error(cube, "Error\ninit_mlx() failed");
	if (init_mlx(&cube->mlx_test) == -1)
		exit_error(cube, "Error\ninit_mlx() failed");
	if (init_textures(cube, map_path) == -1)
		exit_error(cube, "Error\ninit_textures() failed");
}
