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

	i = 0;
	visited = (bool **)malloc(map->h * sizeof(bool *));
	while (i < map->h)
	{
		visited[i] = (bool *)ft_calloc(map->w, sizeof(bool));
		i++;
	}
	player_x = state->pos.x;
	player_y = state->pos.y;
	printf("Inizio flood fill da: (%d, %d)\n", player_x, player_y);
	result = flood_fill(map, player_x, player_y, visited);
	i = 0;
	while (i < map->h)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
	return (result);
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
    if (!line || !state || !player_count) return; // Add null check
	size_t	col;
	size_t	actual_col;

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
		if (line[col] == '\t')
			actual_col += 4;
		else
			actual_col++;
		col++;
	}
}

void	parse_player(t_state *state, const char *map_path)
{
    int		fd;
    char	*line;
    int		row;
    int		player_count;

    player_count = 0;
    fd = open(map_path, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening map file");
        exit(1);
    }
    skip_texture_info(fd);
    row = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        parse_line(state, line, row, &player_count);
        free(line);
        row++;
    }
    if (player_count != 1)
    {
        printf("\033[0;31mError: Invalid number of players\033[0m\n");
        exit(1);
    }
    close(fd);
}
