/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:13:51 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 22:49:06 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	is_map_enclosed(t_state *state, t_map *map)
{
	bool	**visited;
	bool	result;
	int		pgx;
	int		pgy;

	if (!state || !map || map->w <= 0 || map->h <= 0)
		return (false);
	visited = allocate_visited(map->w, map->h);
	if (!visited)
		return (false);
	if (!is_player_position_valid(state, map))
	{
		free_visited(visited, map->h);
		return (false);
	}
	pgx = (int)state->pos.x;
	pgy = (int)state->pos.y;
	printf("\033[0;32m[DEBUG] Inizio FF da: (%d, %d)\033[0m\n", pgx, pgy);
	result = flood_fill(map, pgx, pgy, visited);
	free_visited(visited, map->h);
	printf("\033[0;34m[DEBUG FF] Flood Fill completato\033[0m\n");
	return (result);
}

void	set_position_and_direction(t_state *state, char dir_c, int col, int row)
{
	if (!state)
		return ;
	state->pos.x = col + 0.5;
	state->pos.y = row + 0.5;
	if (dir_c == 'N')
		get_dir_v(&state->dir, UP);
	else if (dir_c == 'S')
		get_dir_v(&state->dir, DOWN);
	else if (dir_c == 'W')
		get_dir_v(&state->dir, LEFT);
	else if (dir_c == 'E')
		get_dir_v(&state->dir, RIGHT);
}

void	parse_line(t_state *state, const char *line, int row)
{
	size_t	col;
	size_t	actual_col;
	char	c;

	if (!line || !state)
		return ;
	col = 0;
	actual_col = 0;
	while (col < ft_strlen(line))
	{
		c = line[col];
		if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		{
			set_position_and_direction(state, c, actual_col, row);
			break ;
		}
		else
			actual_col += 1;
		col++;
	}
}

int	parse_player(t_state *state, const char *map_path)
{
	int		fd;
	int		row;
	char	*line;

	if (!state || !map_path)
		return (printf("\033[0;31mError\n Invalid state/map\033[0m\n"), (-1));
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error\n opening map file"), (-1));
	line = skip_texture(fd);
	row = 0;
	while (line != NULL)
	{
		parse_line(state, line, row);
		row++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	printf("[DEBUG] Player Pos: (%.2f, %.2f)\n", state->pos.x, state->pos.y);
	return (0);
}

int	init_state(t_state *state, const char *map_path)
{
	if (!state || !map_path)
		return (printf("\033[0;31mError\n Invalid state/path\033[0m\n"), (-1));
	if (parse_player(state, map_path) == -1)
		return (printf("\033[0;31mError\n Failed to parse PG\033[0m\n"), (-1));
	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;
	v_perp(&state->plane, state->dir);
	v_mul(&state->plane, state->plane, tan(FOV / 2));
	printf("\033[0;32m[DEBUG STATE] State Initialized\033[0m\n");
	return (0);
}
