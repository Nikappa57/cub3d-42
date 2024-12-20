/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:11:49 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 16:16:06 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3D.h"

static void	draw_dir(
	t_cub3d *cube, t_point	pos, t_vector pos_dir, int block_size)
{
	t_point	pos_dir_screen;

	vector_to_screen(pos_dir, &pos_dir_screen, block_size);
	draw_line(&cube->mlx_test.data, pos, pos_dir_screen, RED);
}

static void	draw_rays(t_cub3d *cube, t_point pos, int block_size)
{
	t_point		hit_screen;
	t_vector	hit;
	int			x;

	x = -1;
	while (++x < WIN_WIDTH)
	{
		dda_distance(cube, x);
		if (cube->dda.side != -1)
		{
			v_mul(&hit, cube->dda.ray_dir, cube->dda.distance);
			v_sum(&hit, hit, cube->state.pos);
			vector_to_screen(hit, &hit_screen, block_size);
			draw_line(&cube->mlx_test.data, pos, hit_screen, YELLOW);
		}
	}
}

static void	draw_plane(t_cub3d *cube, t_vector pos_dir, int block_size)
{
	t_vector	camera1;
	t_vector	camera2;
	t_point		camera1_screen;
	t_point		camera2_screen;

	v_sub(&camera1, pos_dir, cube->state.plane);
	v_sum(&camera2, pos_dir, cube->state.plane);
	vector_to_screen(camera1, &camera1_screen, block_size);
	vector_to_screen(camera2, &camera2_screen, block_size);
	draw_line(&cube->mlx_test.data, camera1_screen, camera2_screen, BLUE);
}

void	draw_map(t_cub3d *cube)
{
	int			block_size;
	t_point		pos;
	t_vector	pos_dir;

	block_size = (int)fmin(WIN_WIDTH / cube->map.w, WIN_HEIGHT / cube->map.h);
	draw_wall_block(cube, block_size);
	draw_grid(cube, block_size);
	vector_to_screen(cube->state.pos, &pos, block_size);
	draw_pos(cube, pos, block_size);
	v_sum(&pos_dir, cube->state.pos, cube->state.dir);
	draw_dir(cube, pos, pos_dir, block_size);
	draw_plane(cube, pos_dir, block_size);
	draw_rays(cube, pos, block_size);
}
