/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:04:20 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 14:47:38 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	draw_xy(t_img *img, int x, int y, t_color color)
{
	put_pixel(img, x, y, color);
}

inline void	draw_point(t_img *img, t_point point, t_color color)
{
	draw_xy(img, point.x, point.y, color);
}

static void	get_sign(t_point *sign, t_point start, t_point end)
{
	if (start.x < end.x)
		sign->x = 1;
	else
		sign->x = -1;
	if (start.y < end.y)
		sign->y = 1;
	else
		sign->y = -1;
}

void	draw_line(t_img *img, t_point start, t_point end, t_color color)
{
	t_point	delta;
	t_point	sign;
	t_point	error;

	delta.x = abs(end.x - start.x);
	delta.y = abs(end.y - start.y);
	get_sign(&sign, start, end);
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
	draw_point(img, end, color);
}

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
