# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lorenzogaudino <lorenzogaudino@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/25 12:04:15 by lgaudino          #+#    #+#              #
#    Updated: 2023/03/23 12:01:29 by lorenzogaud      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				=	libft.a

DIR_SRCS			=	srcs
DIR_OBJS			=	objs
SUBDIRS				=	ft_is ft_to ft_mem ft_str ft_put ft_lst ft_len ft_printf gnl

SRCS_PATHS			=	$(foreach dir, $(SUBDIRS), $(addprefix $(DIR_SRCS)/, $(dir)))
OBJS_PATHS			=	$(foreach dir, $(SUBDIRS), $(addprefix $(DIR_OBJS)/, $(dir)))
SRCS				=	$(foreach dir, $(SRCS_PATHS), $(wildcard $(dir)/*.c))
OBJS				=	$(subst $(DIR_SRCS), $(DIR_OBJS), $(SRCS:.c=.o))

INCLUDES			=	-I includes

CC					=	gcc
CFLAGS				=	-Wall -Wextra -Werror
RM					=	/bin/rm -f

$(DIR_OBJS)/%.o :	$(DIR_SRCS)/%.c
					mkdir -p $(DIR_OBJS) $(OBJS_PATHS)
					$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all:				$(NAME)

$(NAME):			$(OBJS)
					ar -rcs $(NAME) $(OBJS)

clean:
					$(RM) $(OBJS)
					$(RM) -r $(DIR_OBJS)

fclean:				clean
					$(RM) $(NAME)

re:					fclean all

.PHONY:				all clean fclean re