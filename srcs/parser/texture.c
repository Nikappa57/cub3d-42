/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:55 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/20 16:15:13 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	parse_color(const char *str)
{
	int		r;
	int		g;
	int		b;

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
	if (*str != '\0' && *str != '\n')
		return (-1);
	printf("\033[0;33mColor parsed: %d, %d, %d\033[0m\n", r, g, b);

	return ((r << 16) | (g << 8) | b);
}

int	read_config(const char *file_path, t_config *config)
{
	FILE	*file;
	char	line[256];
	char	key[3];
	char	value[256];
	char	*trimmed_line;

	file = fopen(file_path, "r");
	if (!file)
		return (printf("Error: Failed to open config file %s\n",
				file_path), (-1));
	while (fgets(line, sizeof(line), file))
	{
		trimmed_line = skip_spaces_and_tabs(line);
		if (sscanf(trimmed_line, "%2s %255[^\n]", key, value) == 2)
		{
			if (strcmp(key, "NO") == 0)
				config->north_texture = strdup(value);
			else if (strcmp(key, "SO") == 0)
				config->south_texture = strdup(value);
			else if (strcmp(key, "WE") == 0)
				config->west_texture = strdup(value);
			else if (strcmp(key, "EA") == 0)
				config->east_texture = strdup(value);
			else if (strcmp(key, "F") == 0)
				config->floor_color = parse_color(value);
			else if (strcmp(key, "C") == 0)
				config->ceiling_color = parse_color(value);
		}
	}
	fclose(file);
	return (0);
}

int	load_texture(t_cub3d *cube, t_img *t, const char *path)
{
	printf("Attempting to load texture from path: %s\n", path);
	t->img = mlx_xpm_file_to_image(cube->mlx.mlx,
			(char *)path, &t->img_width, &t->img_height);
	if (!t->img)
		return (printf("\033[0;31mError: Failed to load texture %s\n\033[0m",
				path), (-1));
	printf("\033[0;32mTexture loaded: %s\n\033[0m", path);
	t->addr = mlx_get_data_addr(t->img,
			&t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr)
		return (printf("\033[0;31mError: Failed data for texture %s\n\033[0m",
				path), (-1));
	printf("\033[0;32mData address obtained successfully for texture: %s\n\033[0m", path);
	return (0);
}
