/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:12:50 by lorenzogaud       #+#    #+#             */
/*   Updated: 2024/12/20 14:53:16 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "colors.h"
# include "events.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <mlx.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <stdbool.h>
# include <fcntl.h>
# include <string.h>
# include <ctype.h>

/* macro */
# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

/* Constants */

# define WIN_WIDTH			800
# define WIN_HEIGHT			600
# define WIN_TITLE			"cub3D"
# define FOV				1.152f
# define MOV_VEL			0.01f
# define ROT_VEL			0.0087
# define INFINITY_VALUE		1e30
# define MIN_DISTANCE		0.01

/* data structure */

// direction

typedef enum e_dir
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE_DIR
}	t_dir;

typedef enum e_rot
{
	NONE_ROT,
	LEFT_ROT,
	RIGHT_ROT
}	t_rot;

// 2d vector
typedef struct s_vector
{
	double	x;
	double	y;
}			t_vector;

// 2d point
typedef struct s_point
{
	int		x;
	int		y;
}			t_point;

typedef struct s_img
{
	void		*img;
	char		*addr;
	int			img_width;
	int			img_height;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}	t_img;

typedef struct s_mlx
{
	void		*mlx;
	void		*win;
	t_img		data;
}				t_mlx;

typedef struct s_map
{
	int			**m;
	int			w;
	int			h;
}				t_map;

typedef struct s_state
{
	t_vector	pos;
	t_vector	dir;
	t_vector	plane;
	t_dir		move_x;
	t_dir		move_y;
	t_rot		rot;
}				t_state;

typedef struct s_dda
{
	t_vector	ray_dir;
	t_vector	start_pos;
	t_point		map_pos;
	double		delta_dist_x;
	double		delta_dist_y;
	double		side_dist_x;
	double		side_dist_y;
	int			step_x;
	int			step_y;
	int			side;
	double		distance;
}	t_dda;

typedef struct s_cub3D
{
	t_mlx		mlx;
	t_mlx		mlx_test;
	t_map		map;
	t_img		texture[4];
	t_color		floor_color;
	t_color		ceiling_color;
	t_state		state;
	t_dda		dda;
}				t_cub3d;

typedef struct s_config
{
	char		*north_texture;
	char		*south_texture;
	char		*west_texture;
	char		*east_texture;
	int			floor_color;
	int			ceiling_color;
}				t_config;

/*************** Funtions ***************/

//pars
void		init_cube(t_cub3d *cube, const char *map_path);
void		set_hook(t_cub3d *cube);

//texture
int			load_texture(t_cub3d *cube, t_img *t, const char *path);
int			init_textures(t_cub3d *cube, const char *map_path);
int			read_config(const char *map_path, t_config *config);
int			parse_color(const char *line);

//map
int			init_map(t_map *map, const char *map_path);

//player
void		parse_player(t_state *state, const char *map_path);
void		parse_line(t_state *state, const char *line, int row,
				int *player_count);
void		set_position_and_direction(t_state *state, char direction_char,
				int col, int row);
bool		is_map_enclosed(t_state *state, t_map *map);

//utils
void		*ft_calloc(size_t count, size_t size);
bool		flood_fill(t_map *map, int x, int y, bool **visited);
void		skip_texture_info(FILE *file);
void		skip_spaces(const int *line, int *len);
void		free_map(t_map *map);

/* exit */

void		clear_exit(t_cub3d *cube, int exitcode);
void		exit_error(t_cub3d *cube, char *message);
void		exit_perror(t_cub3d *cube, char *message);

/* MLX */

void		put_pixel(t_img *img, int x, int y, int color);
void		draw_xy(t_img *img, int x, int y, t_color color);
void		draw_point(t_img *img, t_point point, t_color color);
void		draw_square(t_img *img, t_point start, int size, t_color color);
int			window_clean(t_mlx *mlx);
void		show_window(t_mlx *mlx);
void		draw_h_line(t_img *img, t_point start, t_point end, t_color color);
void		draw_v_line(t_img *img, t_point start, t_point end, t_color color);

/* CUBE */

int			show_cube(t_cub3d *cube);
int			cube_loop(t_cub3d *cube);
void		dda_distance(t_cub3d *cube, int x);
int			window_bound(int p, int max);
bool		is_wall(t_map map, t_point p);

/* Actions */

int			move(t_cub3d *cube);
bool		v_is_wall(t_map map, t_vector pos);
bool		min_wall_dist(t_map map, t_vector p);
void		check_wall(t_map map, t_vector *pos, t_vector old_pos);

/* UTILS */

// vectors

void		v_sum(t_vector *r, t_vector v1, t_vector v2);
void		v_sub(t_vector *r, t_vector v1, t_vector v2);
void		v_mul(t_vector *r, t_vector v, double n);
void		v_div(t_vector *r, t_vector v, double n);
void		v_rotate(t_vector *r, t_vector v, double angle);
void		v_perp(t_vector *r, t_vector v);
void		get_dir_v(t_vector *r, t_dir dir);
double		v_distance_pow2(t_vector v1, t_vector v2);

/************ TEST ************/
void		draw_line(t_img *img, t_point start, t_point end, t_color color);

#endif
