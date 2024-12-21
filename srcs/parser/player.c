/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:13:51 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 15:22:31 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	is_map_enclosed(t_state *state, t_map *map)
{
	bool	**visited;
	int		i;
	int		player_x, player_y;
	bool	result;

	if (!state || !map || map->w <= 1 || map->h <= 0)
		return (false);

	visited = (bool **)malloc(map->h * sizeof(bool *));
	if (!visited)
		return (false);

	for (i = 0; i < map->h; i++)
	{
		visited[i] = (bool *)ft_calloc(map->w, sizeof(bool));
		if (!visited[i])
		{
			while (--i >= 0) // Libera le righe già allocate
				free(visited[i]);
			free(visited);
			visited = NULL; // Prevent double free
			return (false);
		}
	}

	player_x = (int)state->pos.x;
	player_y = (int)state->pos.y;
	printf("\033[0;34m[DEBUG] Player position: (%d, %d)\033[0m\n", player_x, player_y);

	if (player_x < 0 || player_x >= map->w || player_y < 0 || player_y >= map->h)
	{
		printf("\033[0;31mError: Player position out of bounds\033[0m\n");
		result = false;
	}
	else
	{
		printf("\033[0;32m[DEBUG FF] Inizio flood fill da: (%d, %d)\033[0m\n", player_x, player_y);
		result = flood_fill(map, player_x, player_y, visited);
	}

	// Deallocazione matrice visitata
	for (i = 0; i < map->h; i++)
		free(visited[i]);
	free(visited);
	printf("\033[0;34m[DEBUG FF]Flood Fill completato\033[0m\n");
	return (result);
}

void	set_position_and_direction(t_state *state, char direction_char, int col, int row)
{
	if (!state)
		return;

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
	if (!line || !state || !player_count)
		return;

	size_t col = 0, actual_col = 0;
	while (col < ft_strlen(line))
	{
		char c = line[col];
		if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		{
			(*player_count)++;
			set_position_and_direction(state, c, actual_col, row);
		}
		actual_col += (c == '\t') ? 4 : 1;
		col++;
	}
}

int	parse_player(t_state *state, const char *map_path)
{
	if (!state || !map_path)
		return (printf("\033[0;31mError: Invalid state or map path\033[0m\n"), (-1));
	int fd = open(map_path, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening map file");
		return (-1);
	}
	int row = skip_texture_info(fd), player_count = 0;
	char *line = get_next_line(fd);

	while ((line = get_next_line(fd)) != NULL)
	{
		parse_line(state, line, row, &player_count);
		free(line);
		row++;
	}

	close(fd);

	if (player_count != 1)
	{
		fprintf(stderr, "Error: Invalid number of players (%d found)\n", player_count);
		return (-1);
	}
	printf("\033[0;32m[DEBUG PLAYER] Player position: (%.2f, %.2f)\033[0m\n", state->pos.x, state->pos.y);
	return (0);
}

int	init_state(t_state *state, const char *map_path)
{
	if (!state || !map_path)
	{
		printf("\033[0;31mError: Invalid state or map path\033[0m\n");
		return (-1);
	}

	if (parse_player(state, map_path) == -1)
	{
		printf("\033[0;31mError: Failed to parse player from map\033[0m\n");
		return (-1);
	}

	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;

	// Calcola il vettore piano perpendicolare alla direzione
	v_perp(&state->plane, state->dir);

	// Scala il piano in base al campo visivo
	v_mul(&state->plane, state->plane, tan(FOV / 2));
	printf("\033[0;32m[DEBUG STATE] State Initialized\033[0m\n");
	return (0);
}
