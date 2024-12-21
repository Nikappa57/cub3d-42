/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 22:13:00 by lgaudino          #+#    #+#             */
/*   Updated: 2024/12/21 22:23:30 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	free_ptr(void *ptr)
{
	if (ptr)
		free(ptr);
}

void	free_str_arr(char **ptr)
{
	char **tmp;

	tmp = ptr;
	if (ptr)
	{
		while (*ptr)
		{
			free(*ptr);
			ptr++;
		}
		free(tmp);
	}
}