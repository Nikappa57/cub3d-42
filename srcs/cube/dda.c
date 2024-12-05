/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube->dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:58:25 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/05 17:02:35 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/*
 *	delta_dist = | 1 / dir |
 *	for dir = 0 -> infitiy
 */
static void	dda_delta_dist(t_dda *dda)
{
	if (dda->ray_dir.x == 0)
		dda->delta_dist_x = INFINITY_VALUE;
	else
		dda->delta_dist_x = fabs(1 / dda->ray_dir.x);
	if (dda->ray_dir.y == 0)
		dda->delta_dist_y = INFINITY_VALUE;
	else
		dda->delta_dist_y = fabs(1 / dda->ray_dir.y);
}

/*
 *	Save step for each direction and initial side_dist equal to the
 *	distance from the initial position to the x and y side
 */
static void	dda_inital_data(t_cub3d *cube)
{
	if (cube->dda.ray_dir.x < 0)
	{
		cube->dda.step_x = -1;
		cube->dda.side_dist_x = (cube->state.pos.x - cube->dda.map_pos.x) * cube->dda.delta_dist_x;
	}
	else
	{
		cube->dda.step_x = 1;
		cube->dda.side_dist_x = (cube->dda.map_pos.x + 1.0 - cube->state.pos.x) * cube->dda.delta_dist_x;
	}
	if (cube->dda.ray_dir.y < 0)
	{
		cube->dda.step_y = -1;
		cube->dda.side_dist_y = (cube->state.pos.y - cube->dda.map_pos.y) * cube->dda.delta_dist_y;
	}
	else
	{
		cube->dda.step_y = 1;
		cube->dda.side_dist_y = (cube->dda.map_pos.y + 1.0 - cube->state.pos.y) * cube->dda.delta_dist_y;
	}
	cube->dda.side = -1;
	cube->dda.distance = 0;
}

static void	dda_data_setup(t_cub3d *cube, int x)
{
	v_mul(&cube->dda.ray_dir, cube->state.plane, 2 * x / (double) WIN_WIDTH - 1);
	v_sum(&cube->dda.ray_dir, cube->dda.ray_dir, cube->state.dir);
	cube->dda.map_pos.x = (int)cube->state.pos.x;
	cube->dda.map_pos.y = (int)cube->state.pos.y;
	dda_delta_dist(&cube->dda);
	dda_inital_data(cube);
}

static bool	is_wall(t_map map, t_point p)
{
	return (((p.x < 0)
			|| (p.y < 0)
			|| (p.x >= map.w)
			|| (p.y >= map.h))
		|| (map.m[p.y][p.x] != 0));
}

/*
 *	Calculate distance from wall for the x-window ray
 *	
 *	the result is in dda.distance. Other info is saved in dda
 *	side = 0 -> x side
 *	side = 1 -> y side
 *	side = -1 -> no side, you are in a wall!
 *	1) setup the dda info
 *	2) dda loop
 */
void	dda_distance(t_cub3d *cube, int x)
{
	dda_data_setup(cube, x);
	while (!is_wall(cube->map, cube->dda.map_pos))
	{
		if (cube->dda.side_dist_x < cube->dda.side_dist_y)
		{
			cube->dda.side_dist_x += cube->dda.delta_dist_x;
			cube->dda.map_pos.x += cube->dda.step_x;
			cube->dda.side = 0;
		}
		else
		{
			cube->dda.side_dist_y += cube->dda.delta_dist_y;
			cube->dda.map_pos.y += cube->dda.step_y;
			cube->dda.side = 1;
		}
	}
	if (cube->dda.side == 0)
		cube->dda.distance = cube->dda.side_dist_x - cube->dda.delta_dist_x;
	else if (cube->dda.side == 1)
		cube->dda.distance = cube->dda.side_dist_y - cube->dda.delta_dist_y;
}
