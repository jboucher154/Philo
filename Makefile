# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/25 17:46:01 by jebouche          #+#    #+#              #
#    Updated: 2023/06/06 18:40:12 by jebouche         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := philo

CC := CC

CFLAGS := -Wall -Wextra -Werror -I includes

INCS := includes/philo.h

SRC_DIR := srcs

OBJ_DIR := objs

SRCS := main.c setup.c phil_factory.c phil_routine.c vitals_monitor.c cleanup.c \
		utilities_two.c utilities_one.c

SRCS := $(SRCS:%=$(SRC_DIR)/%)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "Philo created"

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
	@echo "Philo .o files and directory removed"

fclean: clean
	@rm -f $(NAME)
	@echo "Philo executable removed"

re: fclean all
	
.PHONY: all clean fclean re