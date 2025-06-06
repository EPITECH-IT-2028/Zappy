##
## EPITECH PROJECT, 2025
## server
## File description:
## Makefile
##

SRC = ./src/main.cpp \

SRC += ./src/ParsingArgs.cpp \

OBJ = $(SRC:.cpp=.o)

CC = g++

CPPFLAGS = -Iinclude -Wall -Wextra -std=c++20

NAME = zappy_gui

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CPPFLAGS) -o $(NAME) $(OBJ)
	@echo "$(NAME) compiled successfully."

clean:
	@rm -f $(OBJ)
	@echo "Object files cleaned."

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) cleaned."

re: fclean all

.PHONY: all clean fclean re
