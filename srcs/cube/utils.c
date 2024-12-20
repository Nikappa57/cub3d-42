/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:39:37 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 15:18:30 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	window_bound(int p, int max)
{
	if (p < 0)
		return (0);
	if (p >= max)
		return (max - 1);
	return (p);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

void	vector_to_screen(t_vector v, t_point *r, int size)
{
	r->x = (int)(v.x * size);
	r->y = (int)((v.y * size));
	if (r->x < 0)
		r->x = 0;
	if (r->y < 0)
		r->y = 0;
	if (r->x >= WIN_WIDTH)
		r->x = WIN_WIDTH - 1;
	if (r->y >= WIN_HEIGHT)
		r->y = WIN_HEIGHT - 1;
}
