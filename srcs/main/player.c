/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:13:51 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/20 11:19:29 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	is_map_enclosed(t_state *state, t_map *map)
{
	bool	**visited;
	int		i;

	i = 0;
	visited = (bool **)malloc(map->h * sizeof(bool *));
	while (i < map->h)
	{
		visited[i] = (bool *)ft_calloc(map->w, sizeof(bool));
		i++;
	}
	int	player_x = state->pos.x;
	int	player_y = state->pos.y;
	printf("Inizio flood fill da: (%d, %d)\n", player_x, player_y);
	bool	result = flood_fill(map, player_x, player_y, visited);
	i = 0;
	while (i < map->h)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
	return result;
}

void	set_position_and_direction(t_state *state,
	char direction_char, int col, int row)
{
	state->pos.x = col + 0.5;
	state->pos.y = row + 0.5;
	if (direction_char == 'N')
		get_dir_v(&state->dir, UP);
	else if (direction_char == 'S')
		get_dir_v(&state->dir, DOWN);
	else if (direction_char == 'W')
		get_dir_v(&state->dir, LEFT);
	else if (direction_char == 'E')
		get_dir_v(&state->dir, RIGHT);
}

void	parse_line(t_state *state, const char *line, int row, int *player_count)
{
	size_t	col;
	size_t	actual_col;

	actual_col = 0;
	col = 0;
	while (col < strlen(line))
	{
		if (line[col] == 'N' || line[col] == 'S'
			|| line[col] == 'W' || line[col] == 'E')
		{
			(*player_count)++;
			set_position_and_direction(state, line[col], actual_col, row);
		}
		if (line[col] == '\t')
			actual_col += 4;
		else
			actual_col++;
		col++;
	}
}

void	parse_player(t_state *state, const char *map_path)
{
	FILE	*file;
	char	line[256];
	int		row;
	int		player_count;

	player_count = 0;
	file = fopen(map_path, "r");
	skip_texture_info(file);
	row = 0;
	while (fgets(line, sizeof(line), file))
	{
		parse_line(state, line, row, &player_count);
		row++;
	}
	if (player_count != 1)
	{
		printf("\033[0;31mError: Invalid number of players\033[0m\n");
		exit(1);
	}
	fclose(file);
}

