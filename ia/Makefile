##
## EPITECH PROJECT, 2025
## ia
## File description:
## Makefile
##

NAME = zappy_ai

all: $(NAME)

$(NAME):
	@echo "Creating $(NAME) binary..."
	@echo '#!/bin/bash' > $(NAME)
	@echo 'python3 zappyIa.py "$@"' >> $(NAME)
	@chmod +x $(NAME)
	@echo "$(NAME) created successfully."

clean:
	@echo "Nothing to clean."

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) removed."

re: fclean all

.PHONY: all clean fclean re