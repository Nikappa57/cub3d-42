/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:55 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/20 11:16:26 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	parse_color(const char *str) {
	int	r, g, b;
	char	*endptr;

	r = strtol(str, &endptr, 10);
	if (*endptr != ',')
		return -1;
	str = endptr + 1;

	g = strtol(str, &endptr, 10);
	if (*endptr != ',')
		return -1;
	str = endptr + 1;

	b = strtol(str, &endptr, 10);
	if (*endptr != '\0' && *endptr != '\n')
		return -1;
	printf("\033[0;31mColor parsed: %d, %d, %d\033[0m\n", r, g, b);
	return (r << 16) | (g << 8) | b;
}

int	read_config(const char *file_path, t_config *config)
{
	FILE	*file = fopen(file_path, "r");
	if (!file) {
		fprintf(stderr, "Error: Failed to open config file %s\n", file_path);
		return -1;
	}

	char	line[256];
	while (fgets(line, sizeof(line), file)) {
		if (strncmp(line, "NO ", 3) == 0)
			config->north_texture = strdup(line + 3);
		else if (strncmp(line, "SO ", 3) == 0)
			config->south_texture = strdup(line + 3);
		else if (strncmp(line, "WE ", 3) == 0)
			config->west_texture = strdup(line + 3);
		else if (strncmp(line, "EA ", 3) == 0)
			config->east_texture = strdup(line + 3);
		else if (strncmp(line, "F ", 2) == 0)
			config->floor_color = parse_color(line + 2);
		else if (strncmp(line, "C ", 2) == 0)
			config->ceiling_color = parse_color(line + 2);
	}

	fclose(file);
	return 0;
}

int	load_texture(t_cub3d *cube, t_img *t, const char *path)
{
	printf("Attempting to load texture from path: %s\n", path);
	t->img = mlx_xpm_file_to_image(cube->mlx.mlx, (char *)path, &t->img_width, &t->img_height);
	if (!t->img)
	{
		printf("Error: Failed to load texture %s\n", path);
		printf("mlx_xpm_file_to_image returned NULL\n");
		return (-1);
	}
	printf("Texture loaded successfully: %s\n", path);
	t->addr = mlx_get_data_addr(t->img, &t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr)
	{
		printf("Error: Failed to get data address for texture %s\n", path);
		printf("mlx_get_data_addr returned NULL\n");
		return (-1);
	}
	printf("Data address obtained successfully for texture: %s\n", path);
	return (0);
}

