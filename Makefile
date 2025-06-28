GREEN=\033[0;32m
YELLOW=\033[0;33m
RED=\033[0;31m
NC=\033[0m

ZAPPY_IA = zappy_ai
ZAPPY_IA_DIR = ia

ZAPPY_SERVER = zappy_server
ZAPPY_SERVER_DIR = server

ZAPPY_CLIENT = zappy_gui
ZAPPY_CLIENT_DIR = gui
ZAPPY_CLIENT_BUILD_DIR = .build

all: $(ZAPPY_IA) $(ZAPPY_SERVER) $(ZAPPY_CLIENT)

$(ZAPPY_IA):
	@echo "\n$(GREEN)[ OK ] Setting up $(ZAPPY_IA)...$(NC)"
	@make -C $(ZAPPY_IA_DIR)
	@cp $(ZAPPY_IA_DIR)/$(ZAPPY_IA) .

$(ZAPPY_SERVER):
	@echo "\n$(GREEN)[ OK ] Compiling $(ZAPPY_SERVER)...$(NC)"
	@make -C $(ZAPPY_SERVER_DIR)
	@cp $(ZAPPY_SERVER_DIR)/$(ZAPPY_SERVER) .

$(ZAPPY_CLIENT):
	@echo "\n$(GREEN)[ OK ] Compiling $(ZAPPY_CLIENT) with CMake...$(NC)"
	@if [ ! -d $(ZAPPY_CLIENT_BUILD_DIR) ]; then \
		cmake -S $(ZAPPY_CLIENT_DIR) -B $(ZAPPY_CLIENT_BUILD_DIR); \
	fi
	@cd $(ZAPPY_CLIENT_BUILD_DIR) && cmake --build .
	@cp $(ZAPPY_CLIENT_DIR)/$(ZAPPY_CLIENT) .

clean:
	@make -C $(ZAPPY_SERVER_DIR) clean
	@make -C $(ZAPPY_IA_DIR) clean
	@cd $(ZAPPY_CLIENT_BUILD_DIR) && cmake --build . --target clean
	@find . -name "*~" -delete
	@find . -name "*.pyc" -delete
	@rm -f *.gcno
	@rm -f *.gcda

fclean: clean
	@make -C $(ZAPPY_SERVER_DIR) fclean
	@make -C $(ZAPPY_IA_DIR) fclean
	@rm -rf $(ZAPPY_CLIENT_DIR)/$(ZAPPY_CLIENT_BUILD_DIR)
	@rm -rf $(ZAPPY_CLIENT_DIR)/.cache
	@rm -f $(ZAPPY_IA)
	@rm -f $(ZAPPY_SERVER)
	@rm -f $(ZAPPY_CLIENT)

re: fclean all

.PHONY: all clean fclean re $(ZAPPY_IA) $(ZAPPY_SERVER) $(ZAPPY_CLIENT)
