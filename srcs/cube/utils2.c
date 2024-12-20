/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:13:02 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 15:19:18 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

t_color	get_color(t_dda dda)
{
	if ((dda.side == 0) && (dda.ray_dir.x > 0))
		return (RED);
	else if ((dda.side == 0) && (dda.ray_dir.x < 0))
		return (GREEN);
	else if ((dda.side == 1) && (dda.ray_dir.y > 0))
		return (ORANGE);
	else if ((dda.side == 1) && (dda.ray_dir.y < 0))
		return (YELLOW);
	else
		return (BLACK);
}

t_dir	get_dir(t_vector dir, int side)
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

unsigned int	get_pixel_color(t_img *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
}
