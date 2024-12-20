/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:11:49 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 15:16:00 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3D.h"

void	draw_map(t_cub3d *cube)
{
	int block_size = (int)fmin(WIN_WIDTH / cube->map.w, WIN_HEIGHT / cube->map.h);

	t_point start;
	// MAP
	for (int y = 0; y < cube->map.h; y++)
	{
		for (int x = 0; x < cube->map.w; x++)
		{
			start.x = x * block_size;
			start.y = y * block_size;
			if (cube->map.m[y][x] == 1)
				draw_square(&cube->mlx_test.data, start, block_size, WHITE);
			else
				draw_square(&cube->mlx_test.data, start, block_size, BLACK);
		}
	}
	for (int y = 0; y < cube->map.h; y++)
	{
		draw_h_line(&cube->mlx_test.data, (t_point){0, y * block_size}, (t_point){WIN_WIDTH, y * block_size}, GRAY);
	}
	for (int x = 0; x < cube->map.w; x++)
	{
		draw_v_line(&cube->mlx_test.data, (t_point){x * block_size, 0}, (t_point){x * block_size, WIN_HEIGHT}, GRAY);
	}

	// POS
	int pos_dim = block_size/4;
	t_point pos;
	t_point start_pos;
	vector_to_screen(cube->state.pos, &pos, block_size);
	start_pos.x = pos.x - pos_dim / 2;
	start_pos.y = pos.y - pos_dim / 2;
	if (start_pos.x < 0)
		start_pos.x = 0;
	if (start_pos.y < 0)
		start_pos.y = 0;

	draw_square(&cube->mlx_test.data, start_pos, pos_dim, GREEN);

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