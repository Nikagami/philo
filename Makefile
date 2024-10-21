# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/03 14:40:50 by aafounas          #+#    #+#              #
#    Updated: 2024/10/21 13:27:09 by aafounas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ==================== FICHIERS SOURCES ==================== #
SRC =	src/philo.c \
	    src/main.c \
		src/parsing.c

OBJDIR = .obj/

OBJS = ${SRC:%.c=${OBJDIR}%.o}

# ==================== COMMANDES ==================== #
CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread -g
RM = rm -rf

NAME = philo

# ==================== RÈGLES DE COMPILATION ==================== #
all: philo_logo ${NAME}

${OBJDIR}%.o: %.c
	@mkdir -p ${@D}
	@printf "\r${GREEN}Compilation of $(CYAN)philosophers${RESET}..."
	@${CC} ${CFLAGS} -c $< -o $@
	${call progress_bar}

${NAME}: ${OBJS}
	@echo "\n$(CYAN)${NAME}$(RESET) $(GREEN)has been created $(RESET)✔️\n"
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME}

# ==================== NETTOYAGE ==================== #
clean:
	@echo "$(RED)Deleting $(CYAN)${NAME} $(YELLOW)objs$(RESET) ✔️"
	@${RM} ${OBJDIR}

fclean: clean
	@${RM} ${NAME}
	@${RM} a.out

re: fclean all

# ==================== AFFICHE LE NOM DU PROJET ==================== #
philo_logo:
	@printf "${PINK} --- PHILOSOPHERS ---\n"

# ==================== BARRE DE PROGRESSION ==================== #
define color_progress
$(if $(shell test $(1) -gt 66 && echo true),$(GREEN),$(if $(shell test $(1) -gt 33 && echo true),$(YELLOW),$(RED)))
endef

define progress_bar
@$(eval COMPILED=$(shell echo $$(($(COMPILED)+1))))
@$(eval PROGRESS_PERCENT=$(shell echo $$((($(COMPILED) * 100) / $(TOTAL)))))
@printf "$(CYAN)❘"
@for number in $$(seq 1 $(COMPILED)); do printf "$(call color_progress,$(PROGRESS_PERCENT))█"; done
@for number in $$(seq 1 $$(($(TOTAL) - $(COMPILED)))); do printf "$(GRAY)•"; done
@printf "$(CYAN)❘ $(COMPILED)/$(TOTAL)$(END) ✔️\r"
endef

# ==================== VARIABLES ==================== #
TOTAL := ${words ${SRC}}
COMPILED := 0

.PHONY: all clean fclean re philo_logo

# ==================== COULEURS ==================== #
RED 	:= \033[1;31m
GREEN 	:= \033[1;32m
YELLOW 	:= \033[1;33m
PINK	:= \033[4;35m
CYAN 	:= \033[1;36m
RESET 	:= \033[0m
END 	:= \033[0m
