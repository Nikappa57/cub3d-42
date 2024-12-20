/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:13:51 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/20 12:21:53 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	is_map_enclosed(t_state *state, t_map *map)
{
	bool	**visited;
	int		i;
	int		player_x;
	int		player_y;
	bool	result;

	if (!state || !map || map->w <= 0 || map->h <= 0)
		return (false);

	// Allocazione matrice visitata
	visited = (bool **)malloc(map->h * sizeof(bool *));
	if (!visited)
		return (false);

	for (i = 0; i < map->h; i++)
	{
		visited[i] = (bool *)ft_calloc(map->w, sizeof(bool));
		if (!visited[i])
		{
			while (i-- > 0)
				free(visited[i]);
			free(visited);
			return (false);
			return (0);
		}
	}

	player_x = (int)state->pos.x;
	player_y = (int)state->pos.y;

	printf("Inizio flood fill da: (%d, %d)\n", player_x, player_y);
	result = flood_fill(map, player_x, player_y, visited);

	// Deallocazione matrice visitata
	for (i = 0; i < map->h; i++)
		free(visited[i]);
	free(visited);

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
	size_t	col;
	size_t	actual_col;

	if (!line || !state || !player_count)
		return;

	actual_col = 0;
	col = 0;
	while (col < ft_strlen(line))
	{
		if (line[col] == 'N' || line[col] == 'S'
			|| line[col] == 'W' || line[col] == 'E')
		{
			(*player_count)++;
			set_position_and_direction(state, line[col], actual_col, row);
		}
		actual_col += (line[col] == '\t') ? 4 : 1;
		col++;
	}
}
int	parse_player(t_state *state, const char *map_path)
{
	int		fd;
	char	*line;
	int		row;
	int		player_count;

	if (!state || !map_path)
	{
		printf("\033[0;31mError: Invalid state or map path\033[0m\n");
		return (-1);
	}

	player_count = 0;

	// Apri il file della mappa
	fd = open(map_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening map file");
		return (-1);
	}

	// Salta le informazioni sulle texture
	if (skip_texture_info(fd) == -1)
	{
		printf("\033[0;31mError: Failed to skip texture info\033[0m\n");
		close(fd);
		return (-1);
	}

	row = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		// Analizza ogni riga e aggiorna il conteggio dei giocatori
		parse_line(state, line, row, &player_count);
		free(line);
		row++;
	}

	close(fd);

	// Verifica che ci sia esattamente un giocatore
	if (player_count != 1)
	{
		printf("\033[0;31mError: Invalid number of players (%d found)\033[0m\n", player_count);
		return (-1);
	}

	return (0);
}
