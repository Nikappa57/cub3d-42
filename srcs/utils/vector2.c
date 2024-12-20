/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 19:44:21 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 15:18:10 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// perpendicolar vector
void	v_perp(t_vector *r, t_vector v)
{
	r->x = -v.y;
	r->y = v.x;
}

// reference frame: screen coordinates
void	get_dir_v(t_vector *v, t_dir dir)
{
	v->x = 0;
	v->y = 0;
	if (dir == UP)
		v->y = -1;
	else if (dir == DOWN)
		v->y = 1;
	else if (dir == LEFT)
		v->x = -1;
	else if (dir == RIGHT)
		v->x = 1;
}

double	v_distance_pow2(t_vector v1, t_vector v2)
{
	return (pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}
