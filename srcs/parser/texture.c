/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:57:55 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 16:34:38 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	parse_color(const char *str)
{
	int	r, g, b;

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

char	*ft_strcpy(char *dest, const char *src)
{
	char *ptr = dest;

	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return (dest);
}

// Funzione di allocazione e copia
char *allocate_and_copy(const char *src) {
    if (!src)
        return NULL;
    char *dest = (char *)malloc(strlen(src) + 1);
    if (dest) {
        strcpy(dest, src);
        printf("\033[0;33m[DEBUG TEXT] Allocated and copied string: %s\033[0m\n", dest);
    }
    return dest;
}

int	read_config(const char *file_path, t_config *config)
{
	int		fd;
	char	*line;
	char	**split_line;
	char	*trimmed_line;
	int		i = 0;

	if (!file_path || !config)
		return (-1);

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		return (-1);

	while ((line = get_next_line(fd)) != NULL)
	{
		trimmed_line = allocate_and_copy(line);
		if (!trimmed_line)
			continue;

		trimmed_line = skip_spaces_and_tabs(trimmed_line);
		if (!trimmed_line)
			continue;

		split_line = ft_split(trimmed_line, ' ');
		if (!split_line)
			continue;

		if (split_line[0] && split_line[1])
		{
			if (ft_strcmp(split_line[0], "NO") == 0)
				config->north_texture = allocate_and_copy(split_line[1]), i++;
			else if (ft_strcmp(split_line[0], "SO") == 0)
				config->south_texture = allocate_and_copy(split_line[1]), i++;
			else if (ft_strcmp(split_line[0], "WE") == 0)
				config->west_texture = allocate_and_copy(split_line[1]), i++;
			else if (ft_strcmp(split_line[0], "EA") == 0)
				config->east_texture = allocate_and_copy(split_line[1]), i++;
			else if (ft_strcmp(split_line[0], "F") == 0)
				config->floor_color = parse_color(split_line[1]), i++;
			else if (ft_strcmp(split_line[0], "C") == 0)
				config->ceiling_color = parse_color(split_line[1]), i++;
		}
		if(i == 6)
			break;
	}
	close(fd);
	return (0);
}

int	load_texture(t_cub3d *cube, t_img *t, const char *path)
{
	if (!cube || !t || !path)
		return (-1);

	printf("\033[0;33m[DEBUG TEXT]Attempting to load texture from path: %s\033[0m\n", path);

	t->img = mlx_xpm_file_to_image(cube->mlx.mlx,
			(char *)path, &t->img_width, &t->img_height);
	if (!t->img)
	{
		printf("\033[0;31mError: Failed to load texture %s\033[0m\n", path);
		return (-1);
	}

	printf("\033[0;33m[DEBUG TEXT]Texture loaded: %s\033[0m\n", path);

	t->addr = mlx_get_data_addr(t->img,
			&t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr)
	{
		printf("\033[0;31mError: Failed to obtain data address for texture %s\033[0m\n", path);
		mlx_destroy_image(cube->mlx.mlx, t->img);
		return (-1);
	}

	printf("\033[0;32m[DEBUG TEXT]Data address obtained successfully for texture: %s\033[0m\n", path);
	return (0);
}

void	free_textures(t_cub3d *cube)
{
	for (int i = 0; i < 4; i++) {
		if (cube->texture[i].img) {
			mlx_destroy_image(cube->mlx.mlx, cube->texture[i].img);
			cube->texture[i].img = NULL;
		}
	}
}

void	ft_free_texture(t_cub3d *cube, t_config *config)
{
	free_textures(cube);
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
	t_config config;
	printf("\033[0;33m[DEBUG TEXT]Initializing textures\033[0m\n");
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
		ft_free_texture(cube, &config);
		return (-1);
	}

	cube->ceiling_color = config.ceiling_color;
	cube->floor_color = config.floor_color;
	ft_free_texture(cube, &config);
	printf("\033[0;32m[DEBUG TEXT]Textures initialized successfully\033[0m\n");
	return (0);
}
