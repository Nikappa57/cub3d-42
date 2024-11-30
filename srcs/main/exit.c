/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:46:56 by lgaudino          #+#    #+#             */
/*   Updated: 2024/11/28 20:34:10 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void clear_mlx(t_mlx mlx)
{
	if (mlx.mlx)
		return ;
	if (mlx.data.img)
		mlx_destroy_image(mlx.mlx, mlx.data.img);
	if (mlx.win)
		mlx_destroy_window(mlx.mlx, mlx.win);
	mlx_destroy_display(mlx.mlx);
	mlx_loop_end(mlx.mlx);
	free(mlx.mlx);
}

static void clear_map(t_map map)
{
	if (map.m)
	{
		for (int i = 0; i < map.h; i++)
			free(map.m[i]);
		free(map.m);
	}
}

void	clear_exit(t_cub3d *cube, int exitcode)
{
	if (cube)
	{
		clear_mlx(cube->mlx);
		clear_map(cube->map);
	}
	exit(exitcode);
}

