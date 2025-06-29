NAME = zappy_ai
TEST_NAME = zappy_ai_test
TEST_DIR = ia/test
SRC_DIR = ia/src

all: $(NAME)

$(NAME):
	@echo "Creating $(NAME) binary wrapper..."
	@echo '#!/bin/bash' > $(NAME)
	@echo 'python3 $(SRC_DIR)/zappyIa.py "$$@"' >> $(NAME)
	@chmod +x $(NAME)
	@echo "$(NAME) wrapper created successfully."

test: ensure-deps $(TEST_NAME)

$(TEST_NAME):
	@echo "Creating test binary wrapper..."
	@echo '#!/bin/bash' > $(TEST_NAME)
	@echo 'python3 -m coverage erase' >> $(TEST_NAME)
	@echo 'echo "Successfully erased previous coverage data."' >> $(TEST_NAME)
	@echo 'python3 -m coverage run -m unittest discover -s $(TEST_DIR) -p "unitTest.py"' >> $(TEST_NAME)
	@echo 'python3 -m coverage report -m' >> $(TEST_NAME)
	@chmod +x $(TEST_NAME)
	@echo "$(TEST_NAME) wrapper created successfully."

ensure-deps:
	@if ! python3 -c "import coverage" 2>/dev/null; then \
		echo "Installing coverage..."; \
		pip install coverage; \
	fi

coverage-html: ensure-deps
	@echo "Generating HTML coverage report..."
	@python3 -m coverage html
	@echo "HTML coverage report generated in htmlcov/ directory"

clean:
	@echo "Cleaning temporary files..."
	@find . -name "*.pyc" -delete
	@find . -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
	@rm -f .coverage
	@rm -rf htmlcov/
	@rm -f $(TEST_NAME)
	@rm -f $(NAME)

fclean: clean
	@echo "Removing $(NAME) and $(TEST_NAME)..."
	@rm -f $(NAME)
	@rm -f $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test ensure-deps coverage-html