/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 21:09:20 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 21:12:02 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	is_texture_info(const char *line)
{
	const char	*valid_chars = "NOSOWEA\n";
	if(line == NULL)
		return (false);
	while (*line)
	{
		if (strchr(valid_chars, *line) == NULL)
			return (false);
		line++;
	}
	return (true);
}

int	is_map_info(const char *line)
{
	int	has_direction = 0;
	if(line == NULL)
		return (0);
	while (*line)
	{
		if (*line != '0' && *line != '1' && *line != ' ' && !strchr("NSWE", *line))
			break ;
		if (strchr("NSWE", *line))
			has_direction++;
		line++;
	}
	return (has_direction);
}

int	check_cub_file_syntax(const char *file_path)
{
	int		fd = open(file_path, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening .cub file");
		return (false);
	}
	char	*line;
	int		has_dir = 0;
	line = get_next_line(fd);
	while (is_texture_info(line))
	{
		free(line);
		line = get_next_line(fd);
	}
	while(line)
	{
		has_dir += is_map_info(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	printf("\033[0;33m[DEBUG] Map info: %d\033[0m\n", has_dir);
	return (has_dir == 1);
}
