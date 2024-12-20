/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 19:44:21 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 14:53:09 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	v_is_wall(t_map map, t_vector pos)
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
bool	min_wall_dist(t_map map, t_vector p)
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
void	check_wall(t_map map, t_vector *pos, t_vector old_pos)
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
}

void	get_dir_vector(t_dir dir, t_vector *v)
{
	v->x = 0;
	v->y = 0;
	if (dir == UP)
		v->y = 1;
	else if (dir == DOWN)
		v->y = -1;
	else if (dir == LEFT)
		v->x = -1;
	else if (dir == RIGHT)
		v->x = 1;
}
