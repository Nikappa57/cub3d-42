/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:04:20 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 14:50:07 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_v_line(t_img *img, t_point start, t_point end, t_color color)
{
	while (start.y <= end.y)
		draw_xy(img, start.x, start.y++, color);
}

void	draw_h_line(t_img *img, t_point start, t_point end, t_color color)
{
	while (start.x <= end.x)
		draw_xy(img, start.x++, start.y, color);
}

void	draw_square(t_img *img, t_point start, int size, t_color color)
{
	int	x;
	int	y;

	y = start.y;
	while (y < start.y + size)
	{
		x = start.x;
		while (x < start.x + size)
		{
			draw_xy(img, x, y, color);
			x++;
		}
		y++;
	}
}
