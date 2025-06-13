#include "CommandHandler.hpp"
#include "../parser/CommandParser.hpp"
#include <iostream>

handlecommand::CommandHandler::CommandHandler(gui::Map& map) : _map(map) {}

void handlecommand::CommandHandler::handleBct(const std::string& command) {
  try {
    parser::TileUpdate update = parser::CommandParser::parseBct(command);
    _map.getTile(update.x, update.y).resources = update.quantities;
  } catch (const std::exception& e) {
    std::cerr << "Error while handling bct: " << e.what() << "\n";
  }
}
