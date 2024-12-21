/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:34:52 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 19:57:11 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	**split_and_trim_line(char *line)
{
	char	*trimmed_line;
	char	**split_line;

	trimmed_line = allocate_and_copy(line);
	if (!trimmed_line)
		return (NULL);
	trimmed_line = skip_spaces_and_tabs(trimmed_line);
	if (!trimmed_line)
		return (NULL);
	split_line = ft_split(trimmed_line, ' ');
	return (split_line);
}

int	read_config(const char *file_path, t_config *config)
{
	int		fd;
	char	*line;
	char	**split_line;
	int		i;

	i = 0;
	if (!file_path || !config)
		return (-1);
	fd = open_config_file(file_path);
	if (fd == -1)
		return (-1);
	while ((line = get_next_line(fd)) != NULL)
	{
		split_line = split_and_trim_line(line);
		if (!split_line)
			continue ;
		parse_config_line(split_line, config, &i);
		if (i == 6)
			break ;
	}
	close(fd);
	return (0);
}

char	*allocate_and_copy(const char *src)
{
	char	*dest;

	if (!src)
		return (NULL);
	dest = (char *)malloc(strlen(src) + 1);
	if (dest)
	{
		ft_strcpy(dest, src);
		printf("\033[0;33m[DEBUG] Allocated/Copied Str: %s\033[0m", dest);
	}
	return (dest);
}

int	open_config_file(const char *file_path)
{
	int	fd;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		printf("\033[0;31mError: no open Configs%s\033[0m\n", file_path);
	return (fd);
}
