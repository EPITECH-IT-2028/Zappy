ZAPPY_IA_DIR = ia
ZAPPY_SERVER_DIR = server
ZAPPY_CLIENT_DIR = client

ZAPPY_IA = zappy_ai
ZAPPY_SERVER = zappy_server
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
	@echo "Compiling $(ZAPPY_CLIENT)..."
	@make -C $(ZAPPY_CLIENT_DIR)
	@cp $(ZAPPY_CLIENT_DIR)/$(ZAPPY_CLIENT) .

clean:
	@make -C $(ZAPPY_SERVER_DIR) clean
	@make -C $(ZAPPY_CLIENT_DIR) clean
	@find . -name "*~" -delete
	@find . -name "*.pyc" -delete
	@find . -name "__pycache__" -type d -exec rm -rf {} + 2>/dev/null || true
	@rm -f *.gcno
	@rm -f *.gcda

fclean: clean
	@make -C $(ZAPPY_SERVER_DIR) fclean
	@make -C $(ZAPPY_CLIENT_DIR) fclean
	@rm -f $(ZAPPY_IA)
	@rm -f $(ZAPPY_SERVER)
	@rm -f $(ZAPPY_CLIENT)

re: fclean all

.PHONY: all clean fclean re $(ZAPPY_IA) $(ZAPPY_SERVER) $(ZAPPY_CLIENT)
