/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:13:51 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/21 20:09:29 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	is_map_enclosed(t_state *state, t_map *map)
{
	bool	**visited;
	bool	result;
	int		pgx;
	int		pgy;

	if (!state || !map || map->w <= 1 || map->h <= 0)
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

void	parse_line(t_state *state, const char *line, int row, int *player_count)
{
	size_t	col;
	size_t	actual_col;
	char	c;

	if (!line || !state || !player_count)
		return ;
	col = 0;
	actual_col = 0;
	while (col < ft_strlen(line))
	{
		c = line[col];
		if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		{
			(*player_count)++;
			set_position_and_direction(state, c, actual_col, row);
		}
		if (c == '\t')
			actual_col += 4;
		else
			actual_col += 1;
		col++;
	}
}

int	parse_player(t_state *state, const char *map_path)
{
	int		fd;
	int		row;
	int		player_count;
	char	*line;

	if (!state || !map_path)
		return (printf("\033[0;31mError: Invalid state/map\033[0m\n"), (-1));
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error opening map file"), (-1));
	row = skip_texture_info(fd);
	player_count = 0;
	line = get_next_line(fd);
	while ((line = get_next_line(fd)) != NULL)
	{
		parse_line(state, line, row, &player_count);
		free(line);
		row++;
	}
	close(fd);
	if (player_count != 1)
		return (printf("Error: Invalid PG (%d found)\n", player_count), (-1));
	printf("\033[0;32m[DEBUG] Player position: (%.2f, %.2f)\033[0m\n", state->pos.x, state->pos.y);
	return (0);
}

int	init_state(t_state *state, const char *map_path)
{
	if (!state || !map_path)
		return (printf("\033[0;31mError: Invalid state/path\033[0m\n"), (-1));
	if (parse_player(state, map_path) == -1)
		return (printf("\033[0;31mError: Failed to parse PG\033[0m\n"), (-1));
	state->move_x = NONE_DIR;
	state->move_y = NONE_DIR;
	state->rot = NONE_ROT;
	v_perp(&state->plane, state->dir);
	v_mul(&state->plane, state->plane, tan(FOV / 2));
	printf("\033[0;32m[DEBUG STATE] State Initialized\033[0m\n");
	return (0);
}
