/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:12:50 by lorenzogaud       #+#    #+#             */
/*   Updated: 2024/12/05 20:51:58 by lgaudino         ###   ########.fr       */
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

/* macro */
# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define to_rad(angle) angle * M_PI / 180

/* Constants */

# define WIN_WIDTH		800
# define WIN_HEIGHT		600
# define WIN_TITLE		"cub3D"
# define FOV			to_rad(66)
# define MOV_VEL		0.01f
# define ROT_VEL		to_rad(0.5)
# define INFINITY_VALUE	1e30

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

/*************** Funtions ***************/

/* init */

void		init_cube(t_cub3d *cube);
void		set_hook(t_cub3d *cube);
/* exit */

void		clear_exit(t_cub3d *cube, int exitcode);
void		exit_error(t_cub3d *cube, char *message);
void		exit_perror(t_cub3d *cube, char *message);

/* MLX */

void		put_pixel(t_img *img, int x, int y, int color);
void		draw_xy(t_img *img, int x, int y, t_color color);
void		draw_point(t_img *img, t_point point, t_color color);
void		draw_square(t_img *img, int start_x, int start_y, int size, t_color color);
int			window_clean(t_mlx *mlx);
void		show_window(t_mlx *mlx);
void		draw_h_line(t_img *img, t_point start, t_point end, t_color color);
void		draw_v_line(t_img *img, t_point start, t_point end, t_color color);

/* CUBE */

int			show_cube(t_cub3d *cube);
int			cube_loop(t_cub3d *cube);
void		dda_distance(t_cub3d *cube, int x);
int			window_bound(int p, int max);

/* Actions */

int			move(t_cub3d *cube);

/* UTILS */

// vectors

void	v_sum(t_vector *r, t_vector v1, t_vector v2);
void	v_sub(t_vector *r, t_vector v1, t_vector v2);
void	v_mul(t_vector *r, t_vector v, double n);
void	v_div(t_vector *r, t_vector v, double n);
void	v_rotate(t_vector *r, t_vector v, double angle);
void	v_perp(t_vector *r, t_vector v);
void	get_dir_v(t_vector *r, t_dir dir);

/************ TEST ************/
void		draw_line(t_img *img, t_point start, t_point end, t_color color);

#endif