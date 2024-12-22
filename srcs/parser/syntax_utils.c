/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 14:03:17 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/22 15:00:09 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

int	is_valid_number(const char *str)
{
	int	i;

	if (!str || *str == '\0')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_color(const char *str)
{
	char	*token;
	char	buffer[MAX_LINE_LENGTH];
	int		count;
	int		value;

	ft_strncpy(buffer, str, MAX_LINE_LENGTH);
	count = 0;
	token = ft_strtok(buffer, ",");
	while (token)
	{
		if (!is_valid_number(token))
			return (0);
		value = ft_atoi(token);
		if (value < 0 || value > 255)
			return (0);
		count++;
		token = ft_strtok(NULL, ",");
	}
	return (count == 3);
}

int	validate_floor_color(const char *line, t_directives *directives)
{
	if (has_extra_characters(line) || !is_valid_color(line))
	{
		printf("Error\n F color invalid!\n");
		return (0);
	}
	directives->found_f++;
	return (1);
}

int	validate_ceiling_color(const char *line, t_directives *directives)
{
	if (has_extra_characters(line) || !is_valid_color(line))
	{
		printf("Error\n C color invalid\n");
		return (0);
	}
	directives->found_c++;
	return (1);
}
