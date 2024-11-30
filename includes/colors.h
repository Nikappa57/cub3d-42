/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 14:35:31 by lorenzogaud       #+#    #+#             */
/*   Updated: 2024/11/28 17:03:15 by lgaudino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

# define PALETTE_N	3
# define COLOR_N	3

typedef enum e_color
{
	BLACK		= 0x000000,
	WHITE		= 0xFFFFFF,
	RED			= 0xFF0000,
	ORANGE		= 0xFFA500,
	LIME		= 0x00FF00,
	BLUE		= 0x0000FF,
	YELLOW		= 0xFFFF00,
	CYAN		= 0x00FFFF,
	MAGENTA		= 0xFF00FF,
	SILVER		= 0xC0C0C0,
	GRAY		= 0x808080,
	MAROON		= 0x800000,
	OLIVE		= 0x808000,
	GREEN		= 0x008000,
	PURPLE		= 0x800080,
	TEAL		= 0x008080,
	NAVY		= 0x000080,
	VIOLETTE	= 0xE339B8,
	BLUE2		= 0x3947E3
}	t_color;

#endif