/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:55 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 19:55:55 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	parse_color(const char *str)
{
	int	r;
	int	g;
	int	b;

	if (!str)
		return (-1);
	r = ft_atoi(str);
	while (*str && *str != ',')
		str++;
	if (*str != ',')
		return (-1);
	str++;
	g = ft_atoi(str);
	while (*str && *str != ',')
		str++;
	if (*str != ',')
		return (-1);
	str++;
	b = ft_atoi(str);
	while (*str && *str != '\0' && *str != '\n')
		str++;
	if ((*str != '\0' && *str != '\n') || r < 0
		|| r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return ((r << 16) | (g << 8) | b);
}

int	load_texture(t_cub3d *cube, t_img *t, const char *path)
{
	if (!cube || !t || !path)
		return (-1);
	printf("\033[0;33m[DEBUG]Attempting to load texture: %s\033[0m\n", path);
	t->img = mlx_xpm_file_to_image(cube->mlx.mlx,
			(char *)path, &t->img_width, &t->img_height);
	if (!t->img)
	{
		printf("\033[0;31mError: Failed to load texture %s\033[0m\n", path);
		return (-1);
	}
	printf("\033[0;33m[DEBUG]Texture loaded: %s\033[0m\n", path);
	t->addr = mlx_get_data_addr(t->img,
			&t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr)
	{
		printf("\033[0;31mError: Failed to obtain data %s\033[0m\n", path);
		mlx_destroy_image(cube->mlx.mlx, t->img);
		return (-1);
	}
	printf("\033[0;32m[DEBUG]Data address obtained: %s\033[0m\n", path);
	return (0);
}

void	ft_free_texture(t_config *config)
{
	if (config->north_texture)
		free(config->north_texture);
	if (config->south_texture)
		free(config->south_texture);
	if (config->west_texture)
		free(config->west_texture);
	if (config->east_texture)
		free(config->east_texture);
}

int	init_textures(t_cub3d *cube, const char *map_path)
{
	t_config	config;

	printf("\033[0;33m[DEBUG]Initializing textures\033[0m\n");
	if (!cube || !map_path)
		return (printf("Error: Invalid parameters\n"), (-1));
	if (read_config(map_path, &config) == -1)
		return (-1);
	config.north_texture[ft_strcspn(config.north_texture, "\n")] = '\0';
	config.south_texture[ft_strcspn(config.south_texture, "\n")] = '\0';
	config.west_texture[ft_strcspn(config.west_texture, "\n")] = '\0';
	config.east_texture[ft_strcspn(config.east_texture, "\n")] = '\0';
	if (load_texture(cube, &cube->texture[0], config.north_texture) == -1
		|| load_texture(cube, &cube->texture[1], config.south_texture) == -1
		|| load_texture(cube, &cube->texture[2], config.west_texture) == -1
		|| load_texture(cube, &cube->texture[3], config.east_texture) == -1)
	{
		ft_free_texture(&config);
		return (-1);
	}
	cube->ceiling_color = config.ceiling_color;
	cube->floor_color = config.floor_color;
	ft_free_texture(&config);
	printf("\033[0;32m[DEBUG]Textures initialized successfully\033[0m\n");
	return (0);
}
