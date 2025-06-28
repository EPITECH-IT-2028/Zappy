ZAPPY_IA_DIR = ia
ZAPPY_SERVER_DIR = server
ZAPPY_CLIENT_DIR = gui

ZAPPY_IA = zappy_ai
ZAPPY_SERVER = zappy_server

ZAPPY_CLIENT_BUILD_DIR = $(ZAPPY_CLIENT_DIR)/build
ZAPPY_CLIENT = zappy_gui

all: $(ZAPPY_IA) $(ZAPPY_SERVER) $(ZAPPY_CLIENT)

$(ZAPPY_IA):
	@echo "Setting up $(ZAPPY_IA)..."
	@make -C $(ZAPPY_IA_DIR)
	@cp $(ZAPPY_IA_DIR)/$(ZAPPY_IA) ./

$(ZAPPY_SERVER):
	@echo "Compiling $(ZAPPY_SERVER)..."
	@make -C $(ZAPPY_SERVER_DIR)
	@cp $(ZAPPY_SERVER_DIR)/$(ZAPPY_SERVER) .

$(ZAPPY_CLIENT):
	@echo "Compiling $(ZAPPY_CLIENT) with CMake..."
	@if [ ! -d $(ZAPPY_CLIENT_BUILD_DIR) ]; then mkdir -p $(ZAPPY_CLIENT_BUILD_DIR); fi
	@cd $(ZAPPY_CLIENT_BUILD_DIR) && cmake ..
	@cd $(ZAPPY_CLIENT_BUILD_DIR) && make
	@cp $(ZAPPY_CLIENT_DIR)/$(ZAPPY_CLIENT) ./

clean:
	@make -C $(ZAPPY_SERVER_DIR) clean
	@make -C $(ZAPPY_IA_DIR) clean
	@rm -rf $(ZAPPY_CLIENT_BUILD_DIR)
	@find . -name "*~" -delete
	@find . -name "*.pyc" -delete
	@rm -f *.gcno
	@rm -f *.gcda

fclean: clean
	@make -C $(ZAPPY_SERVER_DIR) fclean
	@make -C $(ZAPPY_IA_DIR) fclean
	@rm -f $(ZAPPY_IA)
	@rm -f $(ZAPPY_SERVER)
	@rm -f $(ZAPPY_CLIENT)

re: fclean all

.PHONY: all clean fclean re $(ZAPPY_IA) $(ZAPPY_SERVER) $(ZAPPY_CLIENT)
