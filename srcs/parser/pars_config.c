/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:53:20 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 19:54:49 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	parse_texture_line(char **split_line, t_config *config, int *i)
{
	if (ft_strcmp(split_line[0], "NO") == 0)
	{
		config->north_texture = allocate_and_copy(split_line[1]);
		(*i)++;
	}
	else if (ft_strcmp(split_line[0], "SO") == 0)
	{
		config->south_texture = allocate_and_copy(split_line[1]);
		(*i)++;
	}
	else if (ft_strcmp(split_line[0], "WE") == 0)
	{
		config->west_texture = allocate_and_copy(split_line[1]);
		(*i)++;
	}
	else if (ft_strcmp(split_line[0], "EA") == 0)
	{
		config->east_texture = allocate_and_copy(split_line[1]);
		(*i)++;
	}
}

void	parse_color_line(char **split_line, t_config *config, int *i)
{
	if (ft_strcmp(split_line[0], "F") == 0)
	{
		config->floor_color = parse_color(split_line[1]);
		(*i)++;
	}
	else if (ft_strcmp(split_line[0], "C") == 0)
	{
		config->ceiling_color = parse_color(split_line[1]);
		(*i)++;
	}
}

void	parse_config_line(char **split_line, t_config *config, int *i)
{
	if (split_line[0] && split_line[1])
	{
		parse_texture_line(split_line, config, i);
		parse_color_line(split_line, config, i);
	}
}
