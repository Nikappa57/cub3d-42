/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:43:42 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 19:45:48 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	**allocate_visited(int width, int height)
{
	bool	**visited;
	int		i;

	visited = (bool **)malloc(height * sizeof(bool *));
	if (!visited)
		return (NULL);
	i = 0;
	while (i < height)
	{
		visited[i] = (bool *)ft_calloc(width, sizeof(bool));
		if (!visited[i])
		{
			while (--i >= 0)
				free(visited[i]);
			free(visited);
			return (NULL);
		}
		i++;
	}
	return (visited);
}

void	free_visited(bool **visited, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
}
