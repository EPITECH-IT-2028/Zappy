# Makefile for Zappy AI client
# Creates a wrapper script to run the Python AI client

NAME = zappy_ai

all: $(NAME)

$(NAME):
	@echo "Creating $(NAME) binary wrapper..."
	@echo '#!/bin/bash' > $(NAME)
	@echo 'python3 ia/src/zappyIa.py "$$@"' >> $(NAME)
	@chmod +x $(NAME)
	@echo "$(NAME) wrapper created successfully."

clean:
	@echo "Cleaning temporary files..."
	@rm -f *.pyc __pycache__/*
	@rm -f $(NAME)

fclean: clean
	@echo "Removing $(NAME)..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re