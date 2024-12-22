/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 21:09:20 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/22 15:28:17 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	has_extra_characters(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!isdigit(line[i]) && line[i] != ','
			&& line[i] != ' ' && line[i] != '\n' && line[i] != '\0')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	validate_map_line(const char *line, t_directives *directives)
{
	int	i;

	directives->is_map = 1;
	i = -1;
	while (line[++i])
	{
		if (line[i] == 'N' || line[i] == 'S'
			|| line[i] == 'W' || line[i] == 'E')
			directives->dir_count++;
		else if (line[i] != '1' && line[i] != '0'
			&& line[i] != ' ' && line[i] != '\n')
			return (0);
	}
	return (1);
}

int	check_directives_count(t_directives *directives)
{
	printf("NO: %d\nSO: %d\nWE: %d\nEA: %d\nF: %d\nC: %d\n",
		directives->found_no, directives->found_so, directives->found_we,
		directives->found_ea, directives->found_f, directives->found_c);
	printf("IS_MAP: %d, DIR C: %d\n",
		directives->is_map, directives->dir_count);
	if (directives->found_no != 1 || directives->found_so != 1
		|| directives->found_we != 1 || directives->found_ea != 1
		|| directives->found_f != 1 || directives->found_c != 1
		|| directives->is_map != 1 || directives->dir_count != 1)
	{
		printf("Error\nDirectives error!\n");
		return (0);
	}
	return (1);
}

int	validate_line(const char *line, t_directives *directives)
{
	if (!directives->is_map && ft_strncmp(line, "NO ", 3) == 0)
		directives->found_no++;
	else if (!directives->is_map && ft_strncmp(line, "SO ", 3) == 0)
		directives->found_so++;
	else if (!directives->is_map && ft_strncmp(line, "WE ", 3) == 0)
		directives->found_we++;
	else if (!directives->is_map && ft_strncmp(line, "EA ", 3) == 0)
		directives->found_ea++;
	else if (!directives->is_map && ft_strncmp(line, "F ", 2) == 0)
	{
		if (!validate_floor_color(line + 2, directives))
			return (0);
	}
	else if (!directives->is_map && ft_strncmp(line, "C ", 2) == 0)
	{
		if (!validate_ceiling_color(line + 2, directives))
			return (0);
	}
	else if (*line == '\0' || *line == '\n')
		return (1);
	else if (!validate_map_line(line, directives))
		return (0);
	return (1);
}

int	validate_cub_file(const char *filename)
{
	int				fd;
	char			*line;
	t_directives	directives;

	directives = (t_directives){0, 0, 0, 0, 0, 0, 0, 0};
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (perror("Error\nCannot open file!"), (0));
	line = get_next_line(fd);
	while (line != NULL)
	{
		line[ft_strcspn(line, "\n")] = '\0';
		if (!validate_line(line, &directives))
		{
			printf("Error\nLine %s is invalid!\n", line);
			free(line);
			close(fd);
			return (0);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (check_directives_count(&directives));
}
