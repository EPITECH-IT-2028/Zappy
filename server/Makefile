##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Makefile
##

SRC = ./src/main.c
SRC += ./src/server.c
SRC += ./src/error/error_params.c
SRC += ./src/error/check_clients_nb.c
SRC += ./src/error/check_height.c
SRC += ./src/error/check_width.c
SRC += ./src/error/check_teams_names.c
SRC += ./src/error/check_freq.c
SRC += ./src/error/check_port.c
SRC += ./src/error/help_flag.c
SRC += ./src/memory_management/free_server.c

OBJ = $(SRC:.c=.o)

CC = gcc

CFLAGS = -W -Wall -Wextra -I./include -g

NAME = zappy_server

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(NAME) compiled successfully."

clean:
	@rm -f $(OBJ)
	@echo "Object files cleaned."

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) cleaned."

re: fclean all

.PHONY: all clean fclean re
