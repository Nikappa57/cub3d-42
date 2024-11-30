/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:04:20 by lgaudino          #+#    #+#             */
/*   Updated: 2024/11/30 18:05:40 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_xy(t_mlx_data *img, int x, int y, t_color color)
{
	char	*dst;
	int		offset;

	offset = (y * img->line_length + x * (img->bits_per_pixel / 8));
	dst = img->addr + offset;
	*(unsigned int *)dst = color;
}

inline void	draw_point(t_mlx_data *img, t_point point, t_color color)
{
	draw_xy(img, point.x, point.y, color);
}

void	draw_line(t_mlx_data *img, t_point start, t_point end, t_color color)
{
	t_point	delta;
	t_point	sign;
	t_point	error;

	delta.x = abs(end.x - start.x);
	delta.y = abs(end.y - start.y);
	sign.x = start.x < end.x ? 1 : -1;
	sign.y = start.y < end.y ? 1 : -1;
	error.x = delta.x - delta.y;
	while (start.x != end.x || start.y != end.y)
	{
		draw_point(img, start, color);
		error.y = error.x * 2;
		if (error.y > -delta.y)
		{
			error.x -= delta.y;
			start.x += sign.x;
		}
		if (error.y < delta.x)
		{
			error.x += delta.x;
			start.y += sign.y;
		}
	}
}

void	draw_square(t_mlx_data *img, int start_x, int start_y, int size, t_color color)
{
	int x;
	int y;

	y = start_y;
	while (y < start_y + size)
	{
		x = start_x;
		while (x < start_x + size)
		{
			draw_xy(img, x, y, color);
			x++;
		}
		y++;
	}
}
