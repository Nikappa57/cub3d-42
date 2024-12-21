/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:55 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 10:46:23 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	parse_color(const char *str)
{
	int r, g, b;

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

	if ((*str != '\0' && *str != '\n') || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);

	printf("\033[0;33mColor parsed: %d, %d, %d\033[0m\n", r, g, b);
	return ((r << 16) | (g << 8) | b);
}

int	read_config(const char *file_path, t_config *config)
{
	int		fd;
	char	*line;
	char	**split_line;
	char	*trimmed_line;

	if (!file_path || !config)
		return (-1);

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		printf("\033[0;31mError: Failed to open config file %s\033[0m\n", file_path);
		return (-1);
	}

	while ((line = get_next_line(fd)) != NULL)
	{
		trimmed_line = skip_spaces_and_tabs(line);
		if (!trimmed_line)
		{
			free(line);
			continue;
		}

		split_line = ft_split(trimmed_line, ' ');
		free(trimmed_line);

		if (!split_line)
		{
			free(line);
			continue;
		}

		if (split_line[0] && split_line[1])
		{
			if (ft_strcmp(split_line[0], "NO") == 0)
				config->north_texture = ft_strdup(split_line[1]);
			else if (ft_strcmp(split_line[0], "SO") == 0)
				config->south_texture = ft_strdup(split_line[1]);
			else if (ft_strcmp(split_line[0], "WE") == 0)
				config->west_texture = ft_strdup(split_line[1]);
			else if (ft_strcmp(split_line[0], "EA") == 0)
				config->east_texture = ft_strdup(split_line[1]);
			else if (ft_strcmp(split_line[0], "F") == 0)
				config->floor_color = parse_color(split_line[1]);
			else if (ft_strcmp(split_line[0], "C") == 0)
				config->ceiling_color = parse_color(split_line[1]);
		}

		free(line);
		for (int i = 0; split_line[i]; i++)
			free(split_line[i]);
		free(split_line);
	}

	close(fd);
	return (0);
}

int	load_texture(t_cub3d *cube, t_img *t, const char *path)
{
	if (!cube || !t || !path)
		return (-1);

	printf("Attempting to load texture from path: %s\n", path);

	t->img = mlx_xpm_file_to_image(cube->mlx.mlx,
			(char *)path, &t->img_width, &t->img_height);
	if (!t->img)
	{
		printf("\033[0;31mError: Failed to load texture %s\033[0m\n", path);
		return (-1);
	}

	printf("\033[0;32mTexture loaded: %s\033[0m\n", path);

	t->addr = mlx_get_data_addr(t->img,
			&t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr)
	{
		printf("\033[0;31mError: Failed to obtain data address for texture %s\033[0m\n", path);
		return (-1);
	}

	printf("\033[0;32mData address obtained successfully for texture: %s\033[0m\n", path);
	return (0);
}
