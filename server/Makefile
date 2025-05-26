SRC = ./src/main.c
SRC += ./src/server.c

OBJ = $(SRC:.c=.o)

CC = gcc

CFLAGS = -W -Werror -Wall -Wextra -I./include -g

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

