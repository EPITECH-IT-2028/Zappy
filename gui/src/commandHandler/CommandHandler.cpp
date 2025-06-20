#include "CommandHandler.hpp"
#include <algorithm>
#include <iostream>
#include "entities/Egg.hpp"
#include "entities/Player.hpp"
#include "parser/CommandParser.hpp"

void handlecommand::CommandHandler::handleMsz(const std::string& command) {
  try {
    parser::MapSize mapSize = parser::CommandParser::parseMsz(command);
    _gameState.map.resize(mapSize.width, mapSize.height);
  } catch (const std::exception& e) {
    std::cerr << "Error while handling msz: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handleSgt(const std::string& command) {
  try {
    parser::TimeUnit timeUnit = parser::CommandParser::parseSgt(command);
    _gameState.timeUnit = timeUnit.time;
  } catch (const std::exception& e) {
    std::cerr << "Error while handling sgt: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handleTna(const std::string& command) {
  try {
    parser::TeamNames teamNames = parser::CommandParser::parseTna(command);
    _gameState.teamNames = teamNames.names;
  } catch (const std::exception& e) {
    std::cerr << "Error while handling tna: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handleBct(const std::string& command) {
  try {
    parser::TileUpdate update = parser::CommandParser::parseBct(command);
    if (!_gameState.map.isInside(update.x, update.y)) {
      throw std::out_of_range("Coordinates outside map");
    }
    _gameState.map.getTile(update.x, update.y).resources = update.resources;
  } catch (const std::exception& e) {
    std::cerr << "Error while handling bct: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePnw(const std::string& command) {
  try {
    parser::PlayerInfo playerInfo = parser::CommandParser::parsePnw(command);
    if (!_gameState.map.isInside(playerInfo.x, playerInfo.y)) {
      throw std::out_of_range("Coordinates outside map");
    }

    gui::Player player(playerInfo.id, playerInfo.x, playerInfo.y,
                       playerInfo.orientation, playerInfo.level,
                       playerInfo.teamName);
    _gameState.players.insert_or_assign(player.id, player);

    gui::Tile& tile = _gameState.map.getTile(playerInfo.x, playerInfo.y);
    tile.playerIdsOnTile.push_back(player.id);
  } catch (const std::exception& e) {
    std::cerr << "Error while handling pnw: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePpo(const std::string& command) {
  try {
    parser::PlayerPositionUpdate update =
        parser::CommandParser::parsePpo(command);
    auto player = _gameState.players.find(update.id);

    if (player == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(update.id));
    }
    if (!_gameState.map.isInside(update.x, update.y)) {
      throw std::out_of_range("Coordinates outside map");
    }
    gui::Tile& oldTile =
        _gameState.map.getTile(player->second.x, player->second.y);
    auto& oldPlayerList = oldTile.playerIdsOnTile;
    oldPlayerList.erase(std::remove(oldPlayerList.begin(), oldPlayerList.end(),
                                    player->second.id),
                        oldPlayerList.end());

    player->second.x = update.x;
    player->second.y = update.y;
    player->second.orientation = update.orientation;

    gui::Tile& newTile = _gameState.map.getTile(update.x, update.y);
    newTile.playerIdsOnTile.push_back(player->second.id);
  } catch (const std::exception& e) {
    std::cerr << "Error while handling ppo: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePlv(const std::string& command) {
  try {
    parser::PlayerLevelUpdate update = parser::CommandParser::parsePlv(command);
    auto player = _gameState.players.find(update.id);

    if (player == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(update.id));
    }
    player->second.level = update.level;
  } catch (const std::exception& e) {
    std::cerr << "Error while handling plv: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePin(const std::string& command) {
  try {
    parser::PlayerInventory inventory =
        parser::CommandParser::parsePin(command);
    auto player = _gameState.players.find(inventory.id);

    if (player == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(inventory.id));
    }
    if (!_gameState.map.isInside(inventory.x, inventory.y)) {
      throw std::out_of_range("Coordinates outside map");
    }
    player->second.x = inventory.x;
    player->second.y = inventory.y;
    player->second.inventoryPlayer = inventory.resources;
  } catch (const std::exception& e) {
    std::cerr << "Error while handling pin: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handleEnw(const std::string& command) {
  try {
    parser::EggLaid eggLaid = parser::CommandParser::parseEnw(command);

    if (!_gameState.map.isInside(eggLaid.x, eggLaid.y)) {
      throw std::out_of_range("Coordinates outside map");
    }

    auto playerIt = _gameState.players.find(eggLaid.idPlayer);

    if (playerIt == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(eggLaid.idPlayer));
    }

    const std::string& teamName = playerIt->second.teamName;

    gui::Egg egg(eggLaid.idEgg, eggLaid.x, eggLaid.y, eggLaid.idPlayer,
                 teamName);
    _gameState.eggs.insert_or_assign(egg.id, egg);

    gui::Tile& tile = _gameState.map.getTile(eggLaid.x, eggLaid.y);
    tile.eggIdsOnTile.push_back(egg.id);
  } catch (const std::exception& e) {
    std::cerr << "Error while handling enw: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::removeEgg(int eggId) {
  auto eggIt = _gameState.eggs.find(eggId);
  if (eggIt == _gameState.eggs.end()) {
    throw std::runtime_error("Egg not found with ID " + std::to_string(eggId));
  }
  const gui::Egg& egg = eggIt->second;
  gui::Tile& tile = _gameState.map.getTile(egg.x, egg.y);
  auto& eggList = tile.eggIdsOnTile;
  eggList.erase(std::remove(eggList.begin(), eggList.end(), egg.id),
                eggList.end());
  _gameState.eggs.erase(eggIt);
}

void handlecommand::CommandHandler::handleEbo(const std::string& command) {
  try {
    parser::EggHatch eggHatch = parser::CommandParser::parseEbo(command);
    removeEgg(eggHatch.eggId);
  } catch (const std::exception& e) {
    std::cerr << "Error while handling ebo: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handleEdi(const std::string& command) {
  try {
    parser::EggDeath eggDie = parser::CommandParser::parseEdi(command);
    removeEgg(eggDie.eggId);
  } catch (const std::exception& e) {
    std::cerr << "Error while handling edi: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePdi(const std::string& command) {
  try {
    parser::PlayerDeath playerDeath = parser::CommandParser::parsePdi(command);
    auto playerIt = _gameState.players.find(playerDeath.id);

    if (playerIt == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(playerDeath.id));
    }

    gui::Tile& tile =
        _gameState.map.getTile(playerIt->second.x, playerIt->second.y);
    auto& playerList = tile.playerIdsOnTile;
    playerList.erase(
        std::remove(playerList.begin(), playerList.end(), playerIt->second.id),
        playerList.end());

    _gameState.players.erase(playerIt);
  } catch (const std::exception& e) {
    std::cerr << "Error while handling pdi: " << e.what() << "\n";
  }
}
