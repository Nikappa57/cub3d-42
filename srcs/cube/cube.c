/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:16:39 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 15:18:58 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	get_text_x(t_cub3d *cube, t_img texture)
{
	t_vector	hit;
	double		wall_x;

	v_mul(&hit, cube->dda.ray_dir, cube->dda.distance);
	v_sum(&hit, hit, cube->state.pos);
	if (cube->dda.side == 0)
	{
		wall_x = hit.y - floor(hit.y);
		if (cube->dda.ray_dir.x < 0)
			wall_x = 1 - wall_x;
	}
	else
	{
		wall_x = hit.x - floor(hit.x);
		if (cube->dda.ray_dir.y > 0)
			wall_x = 1 - wall_x;
	}
	return ((int)(wall_x * (double)(texture.img_width - 1)));
}

static void	draw_wall(t_cub3d *cube, t_point start, t_point end,
	int wall_height)
{
	int		y;
	int		text_x;
	double	text_y;
	double	h_step;
	t_img	texture;

	texture = cube->texture[get_dir(cube->dda.ray_dir, cube->dda.side)];
	text_x = get_text_x(cube, texture);
	h_step = (double)texture.img_height / wall_height;
	text_y = ((wall_height - WIN_HEIGHT) / 2 + start.y) * h_step;
	y = start.y;
	while (y <= end.y)
	{
		put_pixel(&cube->mlx.data, start.x, y,
			get_pixel_color(&texture, text_x, (int)text_y));
		y++;
		text_y += h_step;
	}
}

/*
 * 1) floor
 * 2) wall
 * 3) ceiling
 */
static void	draw_x_window(t_cub3d *cube, int x)
{
	int		wall_height;
	t_point	wall_start;
	t_point	wall_end;
	t_point	floor_end;
	t_point	ceiling_start;

	ceiling_start.x = x;
	ceiling_start.y = 0;
	floor_end.x = x;
	floor_end.y = WIN_HEIGHT - 1;
	if (cube->dda.side == -1)
		return (draw_v_line(&cube->mlx.data, ceiling_start, floor_end, BLACK));
	wall_start.x = x;
	wall_end.x = x;
	wall_height = (int)(WIN_HEIGHT / cube->dda.distance);
	wall_start.y = window_bound(WIN_HEIGHT / 2 - wall_height / 2, WIN_HEIGHT);
	wall_end.y = window_bound(WIN_HEIGHT / 2 + wall_height / 2, WIN_HEIGHT);
	if (wall_start.y != 0)
		draw_v_line(&cube->mlx.data, ceiling_start, wall_start,
			cube->ceiling_color);
	draw_wall(cube, wall_start, wall_end, wall_height);
	if (wall_end.y != WIN_HEIGHT - 1)
		draw_v_line(&cube->mlx.data, wall_end, floor_end, cube->floor_color);
}

int	show_cube(t_cub3d *cube)
{
	int	x;

	x = -1;
	while (++x < WIN_WIDTH)
	{
		dda_distance(cube, x);
		draw_x_window(cube, x);
	}
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
