##
## EPITECH PROJECT, 2025
## server
## File description:
## Makefile
##

SRC = ./src/main.c \
			./src/communications/server.c \
			./src/communications/init_struct.c \
			./src/communications/events_server.c \
			./src/communications/connection_commands.c \
			./src/utils.c \
			./src/memory.c \
			./src/game/game.c
SRC += ./src/error/error_params.c
SRC += ./src/error/check_clients_nb.c
SRC += ./src/error/check_height.c
SRC += ./src/error/check_width.c
SRC += ./src/error/check_teams_names.c
SRC += ./src/error/check_freq.c
SRC += ./src/error/check_port.c
SRC += ./src/error/help_flag.c

OBJ = $(SRC:.c=.o)

TEST_SRC = $(filter-out ./src/main.c, $(SRC))
TEST_SRC += ./tests/test_help_flag.c
TEST_SRC += ./tests/test_check_clients_nb.c

TEST_OBJ = $(TEST_SRC:.c=.o)

TEST_NAME = unit_tests
TEST_CFLAGS = -W -Wall -Wextra -I./include -g --coverage -lcriterion

CC = gcc
CFLAGS = -W -Wall -Wextra -I./include -g

NAME = zappy_server

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(NAME) compiled successfully."

$(TEST_OBJ): %.o: %.c
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ) $(TEST_OBJ)
	@rm -f unit_tests*
	@find . -name "*.gcno" -delete
	@find . -name "*.gcda" -delete
	@rm -rf gcovr
	@echo "Object files and coverage files cleaned."

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) cleaned."

unit_tests: $(TEST_OBJ)
	@$(CC) -o $(TEST_NAME) $(TEST_OBJ) $(TEST_CFLAGS)

tests_run: unit_tests
	-./$(TEST_NAME) --verbose --full-stats -j8
	@mkdir -p gcovr
	@gcovr --exclude=tests --html-details gcovr/output.html --print-summary
	@echo "Coverage report generated in gcovr/output.html"

re: fclean all

.PHONY: all clean fclean re unit_tests tests_run
