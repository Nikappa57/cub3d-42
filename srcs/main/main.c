/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:11:01 by lorenzogaud       #+#    #+#             */
/*   Updated: 2024/11/30 23:46:07 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	main(void)
{
	t_cub3d	cube;

	init_cube(&cube);
	set_hook(&cube);
	show_cube(&cube);
	return (mlx_loop(cube.mlx.mlx));
}
