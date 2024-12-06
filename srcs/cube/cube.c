/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:16:39 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/03 21:05:01 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3D.h"

/*
 *	Convert a point from the map to the screen
 *	return 1 if the point is in the screen, 0 otherwise
 */
void	point_to_screen(t_point p, t_point *r, int width, int height, int size)
{
	r->x = p.x * size;
	r->y = (p.y * size);
	if (r->x < 0)
		r->x = 0;
	if (r->y < 0)
		r->y = 0;
	if (r->x >= width)
		r->x = width - 1;
	if (r->y >= height)
		r->y = height - 1;
}

void vector_to_screen(t_vector v, t_point *r, int width, int height, int size)
{
	r->x = (int)(v.x * size);
	r->y = (int)((v.y * size));
	if (r->x < 0)
		r->x = 0;
	if (r->y < 0)
		r->y = 0;
	if (r->x >= width)
		r->x = width - 1;
	if (r->y >= height)
		r->y = height - 1;
}

bool	is_wall(t_map map, int x, int y)
{
	return (((x < 0)
			|| (y < 0)
			|| (x >= map.w)
			|| (y >= map.h))
		|| (map.m[y][x] > 0));
}


// only for test
void	draw_map(t_cub3d *cube)
{
	int block_size = (int)fmin(WIN_WIDTH / cube->map.w, WIN_HEIGHT / cube->map.h);

	// MAP
	for (int y = 0; y < cube->map.h; y++)
	{
		for (int x = 0; x < cube->map.w; x++)
		{
			if (cube->map.m[y][x] == 1)
			{
				draw_square(&cube->mlx_test.data, x * block_size, y * block_size, block_size, WHITE);
			}
		}
	}
	for (int y = 0; y < cube->map.h; y++)
	{
		draw_line(&cube->mlx_test.data, (t_point){0, y * block_size}, (t_point){WIN_WIDTH, y * block_size}, GRAY);
	}
	for (int x = 0; x < cube->map.w; x++)
	{
		draw_line(&cube->mlx_test.data, (t_point){x * block_size, 0}, (t_point){x * block_size, WIN_HEIGHT}, GRAY);
	}

	// POS
	int pos_dim = block_size/4;
	t_point pos;
	vector_to_screen(cube->state.pos, &pos, WIN_WIDTH, WIN_HEIGHT, block_size);
	int start_posx = pos.x - pos_dim / 2;
	int start_posy = pos.y - pos_dim / 2;
	start_posx = start_posx < 0 ? 0 : start_posx;
	start_posy = start_posy < 0 ? 0 : start_posy;
	
	draw_square(&cube->mlx_test.data, start_posx, start_posy, pos_dim, GREEN);

	// DIR
	t_vector pos_dir;
	t_point pos_dir_screen;
	v_sum(&pos_dir, cube->state.pos, cube->state.dir);
	printf("pos: %lf %lf, dir %lf %lf\n", cube->state.pos.x, cube->state.pos.y, cube->state.dir.x, cube->state.dir.y);
	vector_to_screen(pos_dir, &pos_dir_screen, WIN_WIDTH, WIN_HEIGHT, block_size);
	draw_line(&cube->mlx_test.data, pos, pos_dir_screen, RED);

	// PLANE
	t_vector camera1, camera2;
	t_point camera1_screen, camera2_screen;

	v_sub(&camera1, pos_dir, cube->state.plane);
	v_sum(&camera2, pos_dir, cube->state.plane);

	// printf("plane: %lf, %lf\n", cube->state.plane.x, cube->state.plane.y);
	// printf("camera1: %lf %lf camera2: %lf %lf\n", camera1.x, camera1.y, camera2.x, camera2.y);

	vector_to_screen(camera1, &camera1_screen, WIN_WIDTH, WIN_HEIGHT, block_size);
	vector_to_screen(camera2, &camera2_screen, WIN_WIDTH, WIN_HEIGHT, block_size);
	draw_line(&cube->mlx_test.data, camera1_screen, camera2_screen, BLUE);
	// printf("pos screen: %d %d\n", pos.x, pos.y);
	// printf("camera 1 screen: %d %d, camera 2 screen: %d %d\n", camera1_screen.x, camera1_screen.y , camera2_screen.x, camera2_screen.y);

	// DDA
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		t_dda		dda;

		v_mul(&dda.ray_dir, cube->state.plane, 2 * x / (double) WIN_WIDTH - 1);
		v_sum(&dda.ray_dir, dda.ray_dir, cube->state.dir);

		// TEST
		// normalize ray_dir
		// double ray_dir_len = sqrt(dda.ray_dir.x * dda.ray_dir.x + dda.ray_dir.y * dda.ray_dir.y);
		// v_div(&dda.ray_dir, dda.ray_dir, ray_dir_len);

		dda.map_pos.x = (int)cube->state.pos.x;
		dda.map_pos.y = (int)cube->state.pos.y;

		// delta dist
		if (dda.ray_dir.x == 0)
			dda.delta_dist_x = INFINITY_VALUE;
		else
			dda.delta_dist_x = fabs(1 / dda.ray_dir.x);
		if (dda.ray_dir.y == 0)
			dda.delta_dist_y = INFINITY_VALUE;
		else
			dda.delta_dist_y = fabs(1 / dda.ray_dir.y);

		// step and initial side_dist
		if (dda.ray_dir.x < 0)
		{
			dda.step_x = -1;
			dda.side_dist_x = (cube->state.pos.x - dda.map_pos.x) * dda.delta_dist_x;
		}
		else
		{
			dda.step_x = 1;
			dda.side_dist_x = (dda.map_pos.x + 1.0 - cube->state.pos.x) * dda.delta_dist_x;
		}
		if (dda.ray_dir.y < 0)
		{
			dda.step_y = -1;
			dda.side_dist_y = (cube->state.pos.y - dda.map_pos.y) * dda.delta_dist_y;
		}
		else
		{
			dda.step_y = 1;
			dda.side_dist_y = (dda.map_pos.y + 1.0 - cube->state.pos.y) * dda.delta_dist_y;
		}

		// DDA

		dda.side = -1;
		while (!is_wall(cube->map, dda.map_pos.x, dda.map_pos.y))
		{
			if (dda.side_dist_x < dda.side_dist_y)
			{
				dda.side_dist_x += dda.delta_dist_x;
				dda.map_pos.x += dda.step_x;
				dda.side = 0;
			}
			else
			{
				dda.side_dist_y += dda.delta_dist_y;
				dda.map_pos.y += dda.step_y;
				dda.side = 1;
			}
		}

		if (dda.side == 0)
			dda.distance = dda.side_dist_x - dda.delta_dist_x;
		else if (dda.side == 1)
			dda.distance = dda.side_dist_y - dda.delta_dist_y;

		if (dda.side != -1)
		{
			// TEST
			t_vector hit;
			v_mul(&hit, dda.ray_dir, dda.distance);
			v_sum(&hit, hit, cube->state.pos);
			t_point	hit_screen;
			vector_to_screen(hit, &hit_screen, WIN_WIDTH, WIN_HEIGHT, block_size);
			draw_line(&cube->mlx_test.data, pos, hit_screen, YELLOW);
		}

		// draw wall
		int wall_height;
		if (dda.side == -1)
			wall_height = WIN_HEIGHT;
		else
			wall_height = (int)(WIN_HEIGHT / dda.distance);
		int start = WIN_HEIGHT / 2 - wall_height / 2;
		if (start < 0)
			start = 0;
		int end = WIN_HEIGHT / 2 + wall_height / 2;
		if (end >= WIN_HEIGHT)
			end = WIN_HEIGHT - 1;
		draw_line(&cube->mlx.data, (t_point){x, start}, (t_point){x, end}, RED);
	}
}

int	show_cube(t_cub3d *cube)
{
	draw_map(cube);
	show_window(&cube->mlx_test);
	show_window(&cube->mlx);
	return (0);
}

int	cube_loop(t_cub3d *cube)
{
	if (window_clean(&cube->mlx) == -1)
		exit_error(cube, "window_clean() failed");
	if (window_clean(&cube->mlx_test) == -1)
		exit_error(cube, "window_clean() failed");
	if (!move(cube))
		return (0);
	return (show_cube(cube));
}