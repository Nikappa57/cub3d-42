/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:43:42 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 22:48:40 by lgaudino         ###   ########.fr       */
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

bool	is_player_position_valid(t_state *state, t_map *map)
{
	int	pl_x;
	int	pl_y;

	pl_x = (int)state->pos.x;
	pl_y = (int)state->pos.y;
	printf("\033[0;34m[DEBUG] PG position: (%d, %d)\033[0m\n", pl_x, pl_y);
	if (pl_x < 0 || pl_x >= map->w
		|| pl_y < 0 || pl_y >= map->h)
	{
		printf("\033[0;31mError\n Player position out of bounds\033[0m\n");
		return (false);
	}
	return (true);
}
