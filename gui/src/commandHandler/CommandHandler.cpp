#include "CommandHandler.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "entities/Egg.hpp"
#include "entities/Orientation.hpp"
#include "entities/Player.hpp"
#include "parser/CommandParser.hpp"
#include <sstream>

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
    if (timeUnit.time <= 0)
      throw std::invalid_argument("Time unit must be positive");
    _gameState.timeUnit = timeUnit.time;
  } catch (const std::exception& e) {
    std::cerr << "Error while handling sgt: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handleTna(const std::string& command) {
  try {
    parser::TeamNames teamNames = parser::CommandParser::parseTna(command);
    for (const auto& name : teamNames.names) {
      if (std::find(_gameState.teamNames.begin(), _gameState.teamNames.end(),
                    name) == _gameState.teamNames.end()) {
        _gameState.teamNames.push_back(name);
      } else {
        std::cerr << "Duplicate team name found: " << name << "\n";
      }
    }
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
    if (!_gameState.map.isInside(eggLaid.x, eggLaid.y))
      throw std::out_of_range("Coordinates outside map");

    std::string teamName;
    if (eggLaid.idPlayer == -1) {
      teamName = "server";
    } else {
      auto playerIt = _gameState.players.find(eggLaid.idPlayer);
      if (playerIt == _gameState.players.end())
        throw std::runtime_error("Player not found with ID " +
                                 std::to_string(eggLaid.idPlayer));
      teamName = playerIt->second.teamName;
    }

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

    tile.effects.startIncantationEffect();
    gui::IncantationEffect effect(incantation.x, incantation.y,
                                  incantation.level, incantation.playersNumber);

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
                             return effect.x == pie.x && effect.y == pie.y &&
                                    !effect.finished;
                           });

    if (it != _gameState.activeIncantations.end()) {
      it->finished = true;
    }

    tile.effects.stopIncantationEffect();

    if (pie.success)
      tile.effects.showSuccessEffect();
    else
      tile.effects.showFailureEffect();
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
    if (std::find(playerList.begin(), playerList.end(), player.id) ==
        playerList.end()) {
      playerList.push_back(player.id);
    }
    tile.effects.showForkEffect();

  } catch (const std::exception& e) {
    std::cerr << "Error while handling pfk: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePdr(const std::string& command) {
  try {
    parser::DropResource drop = parser::CommandParser::parsePdr(command);

    auto playerIt = _gameState.players.find(drop.playerID);
    if (playerIt == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(drop.playerID));
    }

    gui::Player& player = playerIt->second;
    if (!_gameState.map.isInside(player.x, player.y)) {
      throw std::out_of_range("Player coordinates outside map for pdr event");
    }
    gui::Tile& tile = _gameState.map.getTile(player.x, player.y);
    tile.effects.showDropEffect(drop.resourceNumber);

  } catch (const std::exception& e) {
    std::cerr << "Error while handling pdr: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePgt(const std::string& command) {
  try {
    parser::CollectResource collect = parser::CommandParser::parsePgt(command);

    auto playerIt = _gameState.players.find(collect.playerID);
    if (playerIt == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(collect.playerID));
    }

    gui::Player& player = playerIt->second;
    if (!_gameState.map.isInside(player.x, player.y)) {
      throw std::out_of_range("Player coordinates outside map for pgt event");
    }
    gui::Tile& tile = _gameState.map.getTile(player.x, player.y);
    tile.effects.showCollectEffect(collect.resourceNumber);

  } catch (const std::exception& e) {
    std::cerr << "Error while handling pgt: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePex(const std::string& command) {
  try {
    parser::PlayerExpulsion expulsion =
        parser::CommandParser::parsePex(command);
    auto playerIt = _gameState.players.find(expulsion.playerID);

    if (playerIt == _gameState.players.end()) {
      throw std::runtime_error("Player not found with ID " +
                               std::to_string(expulsion.playerID));
    }

    gui::Player& expellingPlayer = playerIt->second;
    if (!_gameState.map.isInside(expellingPlayer.x, expellingPlayer.y)) {
      throw std::out_of_range("Expelling player coordinates outside map");
    }
    gui::Tile& tile =
        _gameState.map.getTile(expellingPlayer.x, expellingPlayer.y);

    std::vector<int> playersToPush;
    for (int id : tile.playerIdsOnTile) {
      if (id != expulsion.playerID)
        playersToPush.push_back(id);
    }

    for (int id : playersToPush) {
      auto pushedIt = _gameState.players.find(id);
      if (pushedIt == _gameState.players.end())
        continue;

      gui::Player& pushed = pushedIt->second;
      int oldX = pushed.x;
      int oldY = pushed.y;

      switch (expellingPlayer.orientation) {
        case gui::Orientation::NORTH:
          pushed.y = std::max(0, pushed.y - 1);
          break;
        case gui::Orientation::EAST:
          pushed.x = std::min(static_cast<int>(_gameState.map.width - 1),
                              pushed.x + 1);
          break;
        case gui::Orientation::SOUTH:
          pushed.y = std::min(static_cast<int>(_gameState.map.height - 1),
                              pushed.y + 1);
          break;
        case gui::Orientation::WEST:
          pushed.x = std::max(0, pushed.x - 1);
          break;
        default:
          std::cerr << "Unknown orientation: "
                    << static_cast<int>(expellingPlayer.orientation) << "\n";
          break;
      }

      if (pushed.x != oldX || pushed.y != oldY) {
        auto& oldTilePlayers = tile.playerIdsOnTile;
        oldTilePlayers.erase(std::remove(oldTilePlayers.begin(),
                                         oldTilePlayers.end(), pushed.id),
                             oldTilePlayers.end());
        gui::Tile& newTile = _gameState.map.getTile(pushed.x, pushed.y);
        newTile.playerIdsOnTile.push_back(pushed.id);
        newTile.effects.showPushEffect(pushed.id);
      }
    }

  } catch (const std::exception& e) {
    std::cerr << "Error while handling pex: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handlePbc(const std::string& command) {
  try {
    parser::BroadcastEvent event = parser::CommandParser::parsePbc(command);

    auto it = _gameState.players.find(event.playerID);
    if (it == _gameState.players.end())
      throw std::runtime_error("Player not found: ID = " +
                               std::to_string(event.playerID));

    std::ostringstream formatted;
    formatted << "[Player #" << event.playerID << "]: " << event.message;
    _gameState.broadcastLog.push_back(formatted.str());

    if (_gameState.broadcastLog.size() >
        gui::GameState::MAX_BROADCAST_LOG_SIZE) {
      _gameState.broadcastLog.erase(_gameState.broadcastLog.begin());
    }
  } catch (const std::exception& e) {
    std::cerr << "Error while handling pbc: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handleSmg(const std::string& command) {
  try {
    parser::ServerMessageEvent event = parser::CommandParser::parseSmg(command);
    _gameState.broadcastLog.push_back("[Server message] " + event.message);
    while (_gameState.broadcastLog.size() > gui::GameState::MAX_BROADCAST_LOG_SIZE)
      _gameState.broadcastLog.erase(_gameState.broadcastLog.begin());
  } catch (const std::exception& e) {
    std::cerr << "Error while handling smg: " << e.what() << "\n";
  }
}

void handlecommand::CommandHandler::handleSuc(const std::string& command) {
  (void)command;
  _gameState.broadcastLog.push_back("Unknown command received by server.");
  while (_gameState.broadcastLog.size() >
         gui::GameState::MAX_BROADCAST_LOG_SIZE)
    _gameState.broadcastLog.erase(_gameState.broadcastLog.begin());
}

void handlecommand::CommandHandler::handleSbp(const std::string& command) {
  (void)command;
  _gameState.broadcastLog.push_back(
      "Command parameter error reported by server.");
  while (_gameState.broadcastLog.size() >
         gui::GameState::MAX_BROADCAST_LOG_SIZE)
    _gameState.broadcastLog.erase(_gameState.broadcastLog.begin());
}

void handlecommand::CommandHandler::handleSeg(const std::string& command) {
  try {
    parser::GameOverEvent event = parser::CommandParser::parseSeg(command);

    _gameState.isGameOver = true;
    _gameState.winningTeamName = event.winningTeamName;
    _gameState.broadcastLog.push_back("Game Over! Winning team: " + event.winningTeamName);
  } catch (const std::exception& e) {
    std::cerr << "Error parsing seg: " << e.what() << std::endl;
  }
}
