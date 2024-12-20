/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:11:49 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 15:59:09 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3D.h"

static void	draw_grid(t_cub3d *cube, int block_size)
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

static void	draw_wall_block(t_cub3d *cube, int block_size)
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

static void	draw_pos(t_cub3d *cube, t_point	pos, int block_size)
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

void	draw_map(t_cub3d *cube)
{
	int		block_size;
	t_point	pos;

	block_size = (int)fmin(WIN_WIDTH / cube->map.w, WIN_HEIGHT / cube->map.h);
	draw_wall_block(cube, block_size);
	draw_grid(cube, block_size);
	vector_to_screen(cube->state.pos, &pos, block_size);
	draw_pos(cube, pos, block_size);
	

	// DIR
	t_vector pos_dir;
	t_point pos_dir_screen;
	v_sum(&pos_dir, cube->state.pos, cube->state.dir);
	// printf("pos: %lf %lf, dir %lf %lf\n", cube->state.pos.x, cube->state.pos.y, cube->state.dir.x, cube->state.dir.y);
	vector_to_screen(pos_dir, &pos_dir_screen, block_size);
	draw_line(&cube->mlx_test.data, pos, pos_dir_screen, RED);

	// PLANE
	t_vector camera1, camera2;
	t_point camera1_screen, camera2_screen;

	v_sub(&camera1, pos_dir, cube->state.plane);
	v_sum(&camera2, pos_dir, cube->state.plane);

	// printf("plane: %lf, %lf\n", cube->state.plane.x, cube->state.plane.y);
	// printf("camera1: %lf %lf camera2: %lf %lf\n", camera1.x, camera1.y, camera2.x, camera2.y);

	vector_to_screen(camera1, &camera1_screen, block_size);
	vector_to_screen(camera2, &camera2_screen, block_size);
	draw_line(&cube->mlx_test.data, camera1_screen, camera2_screen, BLUE);
	// printf("pos screen: %d %d\n", pos.x, pos.y);
	// printf("camera 1 screen: %d %d, camera 2 screen: %d %d\n", camera1_screen.x, camera1_screen.y , camera2_screen.x, camera2_screen.y);

	// DDA
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		dda_distance(cube, x);

		if (cube->dda.side != -1)
		{
			// TEST
			t_vector hit;
			v_mul(&hit, cube->dda.ray_dir, cube->dda.distance);
			v_sum(&hit, hit, cube->state.pos);
			t_point	hit_screen;
			vector_to_screen(hit, &hit_screen, block_size);
			draw_line(&cube->mlx_test.data, pos, hit_screen, YELLOW);
		}
	}
}