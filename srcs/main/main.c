/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:11:01 by lorenzogaud       #+#    #+#             */
/*   Updated: 2024/12/20 14:44:16 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	main(int argc, char **argv)
{
	t_cub3d	cube;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <map_file.cub>\n", argv[0]);
		return (1);
	}
	init_cube(&cube, argv[1]);
	set_hook(&cube);
	show_cube(&cube);
	return (mlx_loop(cube.mlx.mlx));
}
