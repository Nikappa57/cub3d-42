/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 16:16:12 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/22 14:37:20 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_grid(t_cub3d *cube, int block_size)
{
	int		x;
	int		y;
	t_point	start;
	t_point	end;

	y = -1;
	start.x = 0;
	end.x = cube->map.w * block_size;
	while (++y < cube->map.h)
	{
		start.y = y * block_size;
		end.y = y * block_size;
		draw_h_line(&cube->mlx_test.data, start, end, GRAY);
	}
	x = -1;
	start.y = 0;
	end.y = cube->map.h * block_size;
	while (++x < cube->map.w)
	{
		start.x = x * block_size;
		end.x = x * block_size;
		draw_v_line(&cube->mlx_test.data, start, end, GRAY);
	}
}

void	draw_wall_block(t_cub3d *cube, int block_size)
{
	t_point	start;
	int		x;
	int		y;

	y = -1;
	while (++y < cube->map.h)
	{
		x = -1;
		while (++x < cube->map.w)
		{
			start.x = x * block_size;
			start.y = y * block_size;
			if (cube->map.m[y][x] == 1)
				draw_square(&cube->mlx_test.data, start, block_size, WHITE);
			else
				draw_square(&cube->mlx_test.data, start, block_size, BLACK);
		}
	}
}

void	draw_pos(t_cub3d *cube, t_point	pos, int block_size)
{
	int		pos_dim;
	t_point	start_pos;

	pos_dim = block_size / 4;
	start_pos.x = pos.x - pos_dim / 2;
	start_pos.y = pos.y - pos_dim / 2;
	if (start_pos.x < 0)
		start_pos.x = 0;
	if (start_pos.y < 0)
		start_pos.y = 0;
	draw_square(&cube->mlx_test.data, start_pos, pos_dim, GREEN);
}
