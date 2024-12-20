/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/20 12:04:26 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	init_mlx(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (printf("Error: Failed to initialize mlx\n"), (-1));
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win)
		return (printf("Error: Failed to create window\n"), (-1));
	mlx->data.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->data.img)
		return (printf("Error: Failed to create image\n"), (-1));
	mlx->data.addr = mlx_get_data_addr(mlx->data.img,
			&mlx->data.bits_per_pixel,
			&mlx->data.line_length, &mlx->data.endian);
	if (!mlx->data.addr)
		return (printf("Error: Failed to get data address\n"), (-1));
	return (0);
}

int	init_state(t_state *state, const char *map_path)
{
	parse_player(state, map_path);
	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;
	v_perp(&state->plane, state->dir);
	v_mul(&state->plane, state->plane, tan(FOV / 2));
	return (0);
}

int	init_textures(t_cub3d *cube, const char *map_path)
{
	t_config	config;

	if (read_config(map_path, &config) == -1)
		return (-1);
	config.north_texture[strcspn(config.north_texture, "\n")] = '\0';
	config.south_texture[strcspn(config.south_texture, "\n")] = '\0';
	config.west_texture[strcspn(config.west_texture, "\n")] = '\0';
	config.east_texture[strcspn(config.east_texture, "\n")] = '\0';
	printf("North texture path: %s\n", config.north_texture);
	printf("South texture path: %s\n", config.south_texture);
	printf("West texture path: %s\n", config.west_texture);
	printf("East texture path: %s\n", config.east_texture);
	if (load_texture(cube, &cube->texture[0], config.north_texture) == -1
		|| load_texture(cube, &cube->texture[1], config.south_texture) == -1
		|| load_texture(cube, &cube->texture[2], config.west_texture) == -1
		|| load_texture(cube, &cube->texture[3], config.east_texture) == -1)
		return (-1);
	cube->ceiling_color = config.ceiling_color;
	cube->floor_color = config.floor_color;
	free(config.north_texture);
	free(config.south_texture);
	free(config.west_texture);
	free(config.east_texture);
	return (0);
}

void	init_cube(t_cub3d *cube, const char *map_path)
{
	ft_bzero(cube, sizeof(t_cub3d));
	if (init_map(&cube->map, map_path) == -1)
		exit_error(cube, "init_map() failed");
	if (init_state(&cube->state, map_path) == -1)
		exit_error(cube, "init_state() failed");
	if (!is_map_enclosed(&cube->state, &cube->map))
		exit_error(cube, "Map is not enclosed");
	if (init_mlx(&cube->mlx) == -1)
		exit_error(cube, "init_mlx() failed");
	if (init_mlx(&cube->mlx_test) == -1)
		exit_error(cube, "init_mlx() failed");
	if (init_textures(cube, map_path) == -1)
		exit_error(cube, "init_textures() failed");
}
