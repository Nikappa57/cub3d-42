/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 19:38:11 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 14:52:09 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	position(t_state *state, t_map map)
{
	t_vector	mov;
	t_vector	old_pos;

	old_pos = state->pos;
	v_mul(&mov, state->dir, MOV_VEL);
	if (state->move_y == UP)
		v_sum(&state->pos, state->pos, mov);
	else if (state->move_y == DOWN)
		v_sub(&state->pos, state->pos, mov);
	v_rotate(&mov, mov, M_PI / 2);
	if (state->move_x == RIGHT)
		v_sum(&state->pos, state->pos, mov);
	else if (state->move_x == LEFT)
		v_sub(&state->pos, state->pos, mov);
	if (state->pos.x < 0)
		state->pos.x = 0;
	if (state->pos.x >= map.w)
		state->pos.x = map.w;
	if (state->pos.y < 0)
		state->pos.y = 0;
	if (state->pos.y >= map.h)
		state->pos.y = map.h;
	return (check_wall(map, &state->pos, old_pos),
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
	return (position(&cube->state, cube->map) + rotate(&cube->state));
}
