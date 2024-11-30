/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 19:44:21 by lgaudino          #+#    #+#             */
/*   Updated: 2024/11/30 20:29:50 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
