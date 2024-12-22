/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:11:01 by lorenzogaud       #+#    #+#             */
/*   Updated: 2024/12/22 15:07:18 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	main(int argc, char **argv)
{
	t_cub3d	cube;
	char	*ext;

	if (argc != 2)
	{
		ft_putstr_fd("Usage: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(" <map_file.cub>\n", 2);
		return (1);
	}
	ext = ft_strrchr(argv[1], '.');
	if (!ext || ft_strcmp(ext, ".cub") != 0)
	{
		printf("\033[0;31mError\nThe file must have a .cub extension\033[0m\n");
		return (1);
	}
	init_cube(&cube, argv[1]);
	set_hook(&cube);
	show_cube(&cube);
	return (mlx_loop(cube.mlx.mlx));
}
