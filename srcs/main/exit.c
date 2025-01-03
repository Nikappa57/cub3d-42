/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:46:56 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/21 20:06:48 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	clear_mlx(t_mlx mlx)
{
	if (!mlx.mlx)
		return ;
	if (mlx.data.img)
		mlx_destroy_image(mlx.mlx, mlx.data.img);
	if (mlx.win)
		mlx_destroy_window(mlx.mlx, mlx.win);
	mlx_destroy_display(mlx.mlx);
	mlx_loop_end(mlx.mlx);
	free(mlx.mlx);
}

static void	clear_map(t_map map)
{
	int	i;

	if (map.m)
	{
		i = -1;
		while (++i < map.h)
		{
			if (map.m[i])
				free(map.m[i]);
		}
		free(map.m);
	}
}

static void	clear_texture(t_cub3d cube)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		if (cube.texture[i].img)
			mlx_destroy_image(cube.mlx.mlx, cube.texture[i].img);
	}
}

void	clear_exit(t_cub3d *cube, int exitcode)
{
	if (cube)
	{
		clear_texture(*cube);
		clear_mlx(cube->mlx);
		clear_mlx(cube->mlx_test);
		clear_map(cube->map);
	}
	exit(exitcode);
}
