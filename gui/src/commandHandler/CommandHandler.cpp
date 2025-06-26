#include "CommandHandler.hpp"
#include <algorithm>
#include <iostream>
#include "entities/Egg.hpp"
#include "entities/Player.hpp"
#include "parser/CommandParser.hpp"

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

void handlecommand::CommandHandler::handlePic(const std::string& command) {
  try {
    parser::Incantation incantation = parser::CommandParser::parsePic(command);
    if (!_gameState.map.isInside(incantation.x, incantation.y)) {
      throw std::out_of_range("Coordinates outside map");
    }
    gui::Tile& tile = _gameState.map.getTile(incantation.x, incantation.y);

    if (tile.isEmpty()) {
      throw std::runtime_error("Tile is empty for incantation at (" +
                               std::to_string(incantation.x) + ", " +
                               std::to_string(incantation.y) + ")");
    }

    tile.startIncantationEffect();
    gui::IncantationEffect effect(incantation.x, incantation.y,
                                  incantation.level, incantation.playersNumber);
    effect.x = incantation.x;
    effect.y = incantation.y;
    effect.level = incantation.level;
    effect.players = incantation.playersNumber;

    _gameState.activeIncantations.push_back(effect);

  } catch (const std::exception& e) {
    std::cerr << "Error handling pic: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePie(const std::string& command) {
  try {
    parser::IncantationEnd pie = parser::CommandParser::parsePie(command);
    if (!_gameState.map.isInside(pie.x, pie.y)) {
      throw std::out_of_range("Coordinates outside map");
    }
    gui::Tile& tile = _gameState.map.getTile(pie.x, pie.y);
    auto it = std::find_if(_gameState.activeIncantations.begin(), 
                          _gameState.activeIncantations.end(),
                          [&pie](const gui::IncantationEffect& effect) {
                            return effect.x == pie.x && effect.y == pie.y && !effect.finished;
                          });

    if (it != _gameState.activeIncantations.end()) {
      it->finished = true;
    }

    tile.stopIncantationEffect();

    if (pie.success)
      tile.showSuccessEffect();
    else
      tile.showFailureEffect();
  } catch (const std::exception& e) {
    std::cerr << "Error while handling pie: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePfk(const std::string& command) {
  try {
    parser::ForkEvent forkEvent = parser::CommandParser::parsePfk(command);
    auto playerIt = _gameState.players.find(forkEvent.playerID);

    if (playerIt == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(forkEvent.playerID));
    }

    gui::Player& player = playerIt->second;
    if (!_gameState.map.isInside(player.x, player.y)) {
      throw std::out_of_range("Player coordinates outside map");
    }
    gui::Tile& tile = _gameState.map.getTile(player.x, player.y);

    auto& playerList = tile.playerIdsOnTile;
    if (std::find(playerList.begin(), playerList.end(), player.id) == playerList.end()) {
      playerList.push_back(player.id);
    }
    tile.showForkEffect();

  } catch (const std::exception& e) {
    std::cerr << "Error while handling pfk: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePdr(const std::string& command) {
  try {
    parser::DropResource drop = parser::CommandParser::parsePdr(command);

    auto playerIt = _gameState.players.find(drop.playerID);
    if (playerIt == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " + std::to_string(drop.playerID));
    }

    gui::Player& player = playerIt->second;
    gui::Tile& tile = _gameState.map.getTile(player.x, player.y);
    tile.showDropEffect(drop.resourceNumber);

  } catch (const std::exception& e) {
    std::cerr << "Error while handling pdr: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePgt(const std::string& command) {
  try {
    parser::CollectResource collect = parser::CommandParser::parsePgt(command);

    auto playerIt = _gameState.players.find(collect.playerID);
    if (playerIt == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " + std::to_string(collect.playerID));
    }

    gui::Player& player = playerIt->second;
    gui::Tile& tile = _gameState.map.getTile(player.x, player.y);
    tile.showCollectEffect(collect.resourceNumber);

  } catch (const std::exception& e) {
    std::cerr << "Error while handling pgt: " << e.what() << "\n";
  }
}