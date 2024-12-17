/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:16:39 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/17 16:32:16 by lgaudino         ###   ########.fr       */
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
				draw_square(&cube->mlx_test.data, x * block_size, y * block_size, block_size, WHITE);
			else
				draw_square(&cube->mlx_test.data, x * block_size, y * block_size, block_size, BLACK);
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
	// printf("pos: %lf %lf, dir %lf %lf\n", cube->state.pos.x, cube->state.pos.y, cube->state.dir.x, cube->state.dir.y);
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
		dda_distance(cube, x);

		if (cube->dda.side != -1)
		{
			// TEST
			t_vector hit;
			v_mul(&hit, cube->dda.ray_dir, cube->dda.distance);
			v_sum(&hit, hit, cube->state.pos);
			t_point	hit_screen;
			vector_to_screen(hit, &hit_screen, WIN_WIDTH, WIN_HEIGHT, block_size);
			draw_line(&cube->mlx_test.data, pos, hit_screen, YELLOW);
		}
	}
}

t_color	get_color(t_dda dda)
{
	if ((dda.side == 0) && (dda.ray_dir.x > 0))
		return (RED);
	else if ((dda.side == 0 ) && (dda.ray_dir.x < 0))
		return (GREEN);
	else if ((dda.side == 1 ) && (dda.ray_dir.y > 0))
		return (ORANGE);
	else if ((dda.side == 1) && (dda.ray_dir.y < 0))
		return (YELLOW);
	else
		return (BLACK);
}

static t_dir	get_dir(t_vector dir, int side)
{
	if (side == 0)
	{
		if (dir.x > 0)
			return (RIGHT);
		else
			return (LEFT);
	}
	else
	{
		if (dir.y > 0)
			return (DOWN);
		else
			return (UP);
	}
}

static unsigned int	get_pixel_color(t_img *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
}

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

static void	draw_wall(t_cub3d *cube, t_point start, t_point end, int wall_height)
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
	{
		draw_v_line(&cube->mlx.data, ceiling_start, floor_end, BLACK);
		return ;
	}
	wall_start.x = x;
	wall_end.x = x;
	wall_height = (int)(WIN_HEIGHT / cube->dda.distance);
	wall_start.y = window_bound(WIN_HEIGHT / 2 - wall_height / 2, WIN_HEIGHT);
	wall_end.y = window_bound(WIN_HEIGHT / 2 + wall_height / 2, WIN_HEIGHT);
	if (wall_start.y != 0)
		draw_v_line(&cube->mlx.data, ceiling_start, wall_start, BLUE);
	draw_wall(cube, wall_start, wall_end, wall_height);
	if (wall_end.y != WIN_HEIGHT - 1)
		draw_v_line(&cube->mlx.data, wall_end, floor_end, GRAY);
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
	// if (window_clean(&cube->mlx_test) == -1)
	// 	exit_error(cube, "window_clean() failed");
	if (!move(cube))
		return (0);
	return (show_cube(cube));
}