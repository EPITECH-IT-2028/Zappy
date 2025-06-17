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
			./src/communications/check_player_commands.c \
			./src/communications/gui_commands/map_commands.c \
			./src/communications/gui_commands/time_commands.c \
			./src/communications/gui_commands/player_commands.c \
			./src/events/game_events.c \
			./src/events/connection_events.c \
			./src/utils.c \
			./src/memory.c \
			./src/game/game.c \
			./src/game/init_map.c

SRC += ./src/error/error_params.c
SRC += ./src/error/check_clients_nb.c
SRC += ./src/error/check_height.c
SRC += ./src/error/check_width.c
SRC += ./src/error/check_teams_names.c
SRC += ./src/error/check_freq.c
SRC += ./src/error/check_port.c
SRC += ./src/error/help_flag.c
SRC += ./src/queue.c
SRC += ./src/egg.c
SRC += ./src/game/inventory.c
SRC += ./src/game/look.c
SRC += ./src/game/movement.c
SRC += ./src/game/food.c
SRC += ./src/game/timer_handling.c
SRC += ./src/game/broadcast.c
SRC += ./src/game/fork.c
SRC += ./src/communications/init_direction.c
SRC += ./src/game/connect_nbr.c

OBJ = $(SRC:.c=.o)

TEST_SRC = $(filter-out ./src/main.c, $(SRC))
TEST_SRC += ./tests/test_help_flag.c
TEST_SRC += ./tests/test_check_clients_nb.c
TEST_SRC += ./tests/test_check_freq.c
TEST_SRC += ./tests/test_check_height.c
TEST_SRC += ./tests/test_check_width.c
TEST_SRC += ./tests/test_check_teams_names.c
TEST_SRC += ./tests/test_check_port.c
TEST_SRC += ./tests/test_error_params.c

TEST_OBJ = $(TEST_SRC:.c=.test.o)

TEST_NAME = unit_tests
TEST_BASE_FLAGS = -W -Wall -Wextra -I./include
TEST_CFLAGS = $(TEST_BASE_FLAGS) -lpthread -g --coverage -lcriterion
TEST_LDFLAGS = -lm
TEST_SRC_FILE_NAME = %.test.o

CC = gcc
BASE_FLAGS = -g -W -Wall -Wextra -I./include
CFLAGS = $(BASE_FLAGS) -lpthread
LDFLAGS = -lm

NAME = zappy_server

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(BASE_FLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)
	@echo "$(NAME) compiled successfully."

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule for test object files (with coverage)
$(TEST_SRC_FILE_NAME): %.c
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

# Rule for test files in tests/ directory
./tests/%.o: ./tests/%.c
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

unit_tests: CFLAGS = $(TEST_CFLAGS)
unit_tests: $(TEST_OBJ)
	@$(CC) -o $(TEST_NAME) $(TEST_OBJ) $(TEST_CFLAGS) $(TEST_LDFLAGS)

tests_run: unit_tests
	-./$(TEST_NAME) --verbose --full-stats -j8
	@mkdir -p gcovr
	@gcovr --exclude=tests --html-details gcovr/output.html --print-summary
	@echo "Coverage report generated in gcovr/output.html"

re: fclean all

debug: CFLAGS += -g
debug: $(NAME)

.PHONY: all clean fclean re unit_tests tests_run debug
