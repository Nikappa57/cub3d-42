# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgaudino <lgaudino@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/28 17:04:44 by lgaudino          #+#    #+#              #
#    Updated: 2024/11/30 22:56:26 by lgaudino         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 				=	cub3D

INCLUDES			=	-I includes -I $(LIBFT_DIR)/includes/ -I mlx_linux/

LIBFT				=	$(LIBFT_DIR)libft.a
LIBFT_DIR			=	libft/
LIB_FLAGS			=	-L $(LIBFT_DIR) -lft -L mlx_linux -lmlx_Linux -L /usr/lib -lXext -lX11 -lm -lz

CC					=	gcc
CFLAGS				=	-Wall -Wextra -Werror -g
RM					=	/bin/rm -f
NORM				=	norminette

DIR_SRCS			=	srcs
DIR_OBJS			=	objs
SUBDIRS				=	main mlx_utils cube movement utils

SRCS_PATHS			=	$(foreach dir, $(SUBDIRS), $(addprefix $(DIR_SRCS)/, $(dir)))
OBJS_PATHS			=	$(foreach dir, $(SUBDIRS), $(addprefix $(DIR_OBJS)/, $(dir)))
SRCS				=	$(foreach dir, $(SRCS_PATHS), $(wildcard $(dir)/*.c))
OBJS				=	$(subst $(DIR_SRCS), $(DIR_OBJS), $(SRCS:.c=.o))

$(DIR_OBJS)/%.o :	$(DIR_SRCS)/%.c
					mkdir -p $(DIR_OBJS) $(OBJS_PATHS)
					$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all:				$(NAME)

$(LIBFT):
					make -C $(LIBFT_DIR)

$(NAME):			$(OBJS) $(LIBFT)
					$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB_FLAGS)

clean:
					make clean -C $(LIBFT_DIR)
					$(RM) $(OBJS)
					$(RM) -r $(DIR_OBJS)

fclean:				clean
					make fclean -C $(LIBFT_DIR)
					$(RM) $(NAME)

re:					fclean all

norm:
					$(NORM) includes/*.h srcs/*/*.c

.PHONY:				all clean fclean re norm