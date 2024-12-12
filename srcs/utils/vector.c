/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 19:44:21 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/12 14:17:32 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	v_sum(t_vector *r, t_vector v1, t_vector v2)
{
	r->x = v1.x + v2.x;
	r->y = v1.y + v2.y;
}

void	v_sub(t_vector *r, t_vector v1, t_vector v2)
{
	r->x = v1.x - v2.x;
	r->y = v1.y - v2.y;
}

void	v_mul(t_vector *r, t_vector v, double n)
{
	r->x = v.x * n;
	r->y = v.y * n;
}

void	v_div(t_vector *r, t_vector v, double n)
{
	r->x = v.x / n;
	r->y = v.y / n;
}

/*
 * m = [ [ cos(angle) -sin(angle) ]
 *         [ sin(angle)  cos(angle) ] ]
 *
 * r = m * v
 */
void	v_rotate(t_vector *r, t_vector v, double angle)
{
	r->x = v.x * cos(angle) - v.y * sin(angle);
	r->y = v.x * sin(angle) + v.y * cos(angle);
}

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

