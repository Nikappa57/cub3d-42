/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:27:42 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/21 15:30:01 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void cleanup_resources(t_cub3d *cube) {
    if (cube->mlx.mlx) {
        if (cube->mlx.data.img)
            mlx_destroy_image(cube->mlx.mlx, cube->mlx.data.img);
        if (cube->mlx.win)
            mlx_destroy_window(cube->mlx.mlx, cube->mlx.win);
        for (int i = 0; i < 4; i++) {
            if (cube->texture[i].img)
                mlx_destroy_image(cube->mlx.mlx, cube->texture[i].img);
        }
        mlx_destroy_display(cube->mlx.mlx); // Add this if using Linux
        free(cube->mlx.mlx);
    }
    // Reset pointers to NULL after freeing
    cube->mlx.mlx = NULL;
    cube->mlx.win = NULL;
    cube->mlx.data.img = NULL;
}

int init_mlx(t_mlx *mlx, int width, int height, const char *title)
{
    mlx->mlx = NULL;
    mlx->win = NULL;
    mlx->data.img = NULL;

    mlx->mlx = mlx_init();
    if (!mlx->mlx)
        return (printf("Error: Failed to initialize mlx\n"), (-1));

    mlx->win = mlx_new_window(mlx->mlx, width, height, (char*)title);
    if (!mlx->win)
    {
        cleanup_resources((t_cub3d *)mlx); // Cast to parent struct
        return (printf("Error: Failed to create window\n"), (-1));
    }

    mlx->data.img = mlx_new_image(mlx->mlx, width, height);
    if (!mlx->data.img)
    {
        cleanup_resources((t_cub3d *)mlx);
        return (printf("Error: Failed to create image\n"), (-1));
    }

    mlx->data.addr = mlx_get_data_addr(mlx->data.img,
            &mlx->data.bits_per_pixel,
            &mlx->data.line_length, &mlx->data.endian);
    if (!mlx->data.addr)
    {
        cleanup_resources((t_cub3d *)mlx);
        return (printf("Error: Failed to get data address\n"), (-1));
    }
    return (0);
}

void init_cube(t_cub3d *cube, const char *map_path)
{
    ft_bzero(cube, sizeof(t_cub3d));

    if (init_map(&cube->map, map_path) == -1)
        exit_error(cube, "init_map() failed");

    if (init_state(&cube->state, map_path) == -1)
        exit_error(cube, "init_state() failed");

    if (!is_map_enclosed(&cube->state, &cube->map))
        exit_error(cube, "Map is not enclosed");

    if (init_mlx(&cube->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE) == -1)
        exit_error(cube, "init_mlx() failed");

    if (init_mlx(&cube->mlx_test, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE) == -1)
        exit_error(cube, "init_mlx() failed");

    if (init_textures(cube, map_path) == -1)
    {
        cleanup_resources(cube);
        exit_error(cube, "init_textures() failed");
    }
}

