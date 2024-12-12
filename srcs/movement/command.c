/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 19:38:11 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/12 14:27:58 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static bool	v_is_wall(t_map map, t_vector pos)
{
	t_point	p;
	
	p.x = (int)pos.x;
	p.y = (int)pos.y;
	return (is_wall(map, p));
}
/*
 *	Check if the player hit a wall
 *	return true if the player hit a wall, false otherwise
 *	check the 4 point around the player with a distance of MIN_DISTANCE
 */
static bool	min_wall_dist(t_map map, t_vector p)
{
	if (v_is_wall(map, p))
		return (true);
	p.x += MIN_DISTANCE;
	if (v_is_wall(map, p))
		return (true);
	p.x -= 2 * MIN_DISTANCE;
	if (v_is_wall(map, p))
		return (true);
	p.x += MIN_DISTANCE;
	p.y += MIN_DISTANCE;
	if (v_is_wall(map, p))
		return (true);
	p.y -= 2 * MIN_DISTANCE;
	if (v_is_wall(map, p))
		return (true);
	return (false);
}

/*
 *	Check if the player hit a wall
 *	if the player hit a wall, the player will be moved to the last position
 *	first check the x axis, then the y axis
 */
static void check_wall(t_map map, t_vector *pos, t_vector old_pos)
{
	t_vector	new_pos;

	new_pos = *pos;
	if (!min_wall_dist(map, *pos))
		return ;
	pos->x = old_pos.x;
	if (!min_wall_dist(map, *pos))
		return ;
	pos->x = new_pos.x;
	pos->y = old_pos.y;
	if (!min_wall_dist(map, *pos))
		return ;
	pos->x = old_pos.x;

static int	position(t_cub3d *cube)
{
	t_vector	mov;
	t_vector	old_pos;
	t_state		*state;

	state = &cube->state;
	old_pos = state->pos;
	v_mul(&mov, state->dir, MOV_VEL);
	if (state->move_y == UP)
		v_sum(&state->pos, state->pos, mov);
	else if (state->move_y == DOWN)
		v_sub(&state->pos, state->pos, mov);
	v_rotate(&mov, mov, to_rad(90));
	if (state->move_x == RIGHT)
		v_sum(&state->pos, state->pos, mov);
	else if (state->move_x == LEFT)
		v_sub(&state->pos, state->pos, mov);
	if (state->pos.x < 0)
		state->pos.x = 0;
	if (state->pos.x >= cube->map.w)
		state->pos.x = cube->map.w;
	if (state->pos.y < 0)
		state->pos.y = 0;
	if(is_wall(cube, state->pos))
		state->pos = old_pos;
	if (state->pos.y >= cube->map.h)
		state->pos.y = cube->map.h;
	return (check_wall(cube->map, &state->pos, old_pos),
		(old_pos.x != state->pos.x) || (old_pos.y != state->pos.y));
}

static int	rotate(t_state *state)
{
	if (state->rot == LEFT_ROT)
	{
		v_rotate(&state->dir, state->dir, -ROT_VEL);
		v_rotate(&state->plane, state->plane, -ROT_VEL);
		return (1);
	}
	if (state->rot == RIGHT_ROT)
	{
		v_rotate(&state->dir, state->dir, ROT_VEL);
		v_rotate(&state->plane, state->plane, ROT_VEL);
		return (1);
	}
	return (0);
}

/*
 * Move player
 * return 1 if the player moved, 0 otherwise
 */
int	move(t_cub3d *cube)
{
	return (position(cube) + rotate(&cube->state));
}
