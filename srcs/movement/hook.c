/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 19:22:14 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 14:53:57 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	key_press_hook(int keycode, t_cub3d *cube)
{
	if (keycode == XK_Escape)
		clear_exit(cube, 0);
	else if (keycode == XK_w)
		cube->state.move_y = UP;
	else if (keycode == XK_s)
		cube->state.move_y = DOWN;
	else if (keycode == XK_a)
		cube->state.move_x = LEFT;
	else if (keycode == XK_d)
		cube->state.move_x = RIGHT;
	else if (keycode == XK_Left)
		cube->state.rot = LEFT_ROT;
	else if (keycode == XK_Right)
		cube->state.rot = RIGHT_ROT;
	return (0);
}

static int	key_release_hook(int keycode, t_cub3d *cube)
{
	if ((keycode == XK_w) && (cube->state.move_y == UP))
		cube->state.move_y = NONE_DIR;
	else if ((keycode == XK_s) && (cube->state.move_y == DOWN))
		cube->state.move_y = NONE_DIR;
	else if ((keycode == XK_a) && (cube->state.move_x == LEFT))
		cube->state.move_x = NONE_DIR;
	else if ((keycode == XK_d) && (cube->state.move_x == RIGHT))
		cube->state.move_x = NONE_DIR;
	else if ((keycode == XK_Left) && (cube->state.rot == LEFT_ROT))
		cube->state.rot = NONE_ROT;
	else if ((keycode == XK_Right) && (cube->state.rot == RIGHT_ROT))
		cube->state.rot = NONE_ROT;
	return (0);
}

static int	close_key_hook(int keycode, t_cub3d *cube)
{
	(void)keycode;
	clear_exit(cube, 0);
	return (0);
}

void	set_hook(t_cub3d *cube)
{
	mlx_hook(cube->mlx.win, ON_KEYDOWN, KeyPressMask, key_press_hook, cube);
	mlx_hook(cube->mlx.win, ON_KEYUP, KeyReleaseMask, key_release_hook, cube);
	mlx_hook(cube->mlx.win, ON_DESTROY, 0, close_key_hook, cube);
	mlx_loop_hook(cube->mlx.mlx, &cube_loop, cube);
}
